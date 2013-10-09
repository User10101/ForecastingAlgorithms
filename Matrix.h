#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

namespace Matrix_lib
{
    class NonsquareMatrix {};
    class IndexingError {};
    class Nil {};
    class IncorrectSystem {};

    template <class T> class matrix;
    template <class T>
    bool MakeSquare(matrix<T>&, T);

    template <class T>
    class matrix
    {
        public:
            matrix(int ii, int jj) : m_nrow(ii), m_ncol(jj)
            {
                vector<T> emptyRow;
                for(int i = 0; i < m_ncol; ++i)
                    emptyRow.push_back(T());
                for(int j = 0; j < m_nrow; ++j)
                {
                    m_elems.push_back(emptyRow);
                }
            }

            int Dim1() const { return m_ncol; } // количество элементов в строке
            int Dim2() const { return m_nrow; } // количество строк

            vector<T>& operator[](int n)
            {
                if(n >= m_nrow) throw IndexingError();
                return m_elems[n];
            }

            void Row(vector<T> v, int ind)
            {
                if(v.size() > m_ncol || ind >= m_nrow)
                    throw IndexingError();

                m_elems[ind] = v;
            }
            vector<T> Row(int ind)
            {
                if(ind >= m_nrow)
                    throw IndexingError();
                return m_elems[ind];
            }

            void SwapRows(int f, int s)
            {
                vector<T> temp;
                temp = Row(f);
                Row(Row(s), f);
                Row(temp, s);
            }

            matrix<T>& operator =(matrix<T> m)
            {
                m_elems.resize(m.Dim2());
                m_ncol = m.Dim1();
                m_nrow = m.Dim2();

                for(int i = 0; i < m_nrow; ++i)
                {
                    m_elems[i].resize(m.Dim1());
                }

                for(int j = 0; j < m.Dim2(); ++j)
                {
                    for(int i = 0; i < m.Dim1(); ++i)
                    {
                        m_elems[j][i] = m[j][i];
                    }
                }

                return *this;
            }

            matrix<T> operator *(matrix<T> m)
            {
                matrix<T> result(m.Dim1(), Dim2());

                for(int i = 0; i < Dim2(); ++i)
                {
                    for(int j = 0; j < m.Dim1(); ++j)
                    {
                        for(int k = 0; k < Dim1(); ++k)
                        {
                            result[i][j] += m_elems[i][k]*m[k][j];
                        }
                    }
                }

                return result;
            }

            matrix<T> Inverse();

        private:
            int m_nrow;
            int m_ncol;

            vector< vector<T> > m_elems;

            friend bool MakeSquare<>(matrix<T>&, T);
    };

//---------------------------------------------------------

    template<class T>
    ostream& operator << (ostream& stream, matrix<T> m)
    {
        for(int i = 0; i < m.Dim2(); ++i)
        {
            for(int j = 0; j < m.Dim1(); ++j)
            cout << m[i][j] << ' ';

            cout << endl;
        }

        return stream;
    }

//---------------------------------------------------------

    template<class T>
    ostream& operator << (ostream& stream, vector<T> v)
    {
        for(int i = 0; i < v.size(); ++i)
        {
            cout << v[i] << '\n';
        }

        return stream;
    }

//---------------------------------------------------------

    template<class T>
    istream& operator >> (istream& stream, matrix<T>& m)
    {
        for(int j = 0; j < m.Dim2(); ++j)
        {
            for(int i = 0; i < m.Dim1(); ++i)
            {
                cin >> m[j][i];
            }
        }

        return stream;
    }

//---------------------------------------------------------

    template<class T>
    istream& operator >> (istream& stream, vector<T>& v)
    {
        for(int i = 0; i < v.size(); ++i)
        {
            cin >> v[i];
        }

        return stream;
    }

//---------------------------------------------------------

    template<class T>
    vector<T> ScaleAndAdd(vector<T> primary, T mult, vector<T> result)
    {
        for(int i = 0; i < primary.size(); ++i)
        {
            result[i] = result[i] + primary[i] * mult;
        }

        return result;
    }

    template<class T>
    T DotProduct(vector<T> v1, vector<T> v2)
    {
        if(v1.size() != v2.size()) return 0;

        T sum = 0;
        for(int i = 0; i < v1.size(); ++i)
        {
            sum += v1[i]*v2[i];
        }

        return sum;
    }

    template<class T>
    vector<T> operator * (matrix<T> m, vector<T> v)
    {
        vector<T> result(m.Dim2());

        for(int i = 0; i < m.Dim2(); ++i)
            result[i] = DotProduct(m.Row(i), v);

        return result;
    }

    template<class T>
    vector<T> operator - (vector<T> v1, vector<T> v2)
    {
        for(int i = 0; i < v1.size(); ++i)
        {
            v1[i] -= v2[i];
        }

        return v1;
    }

    template<class T>
    vector<T> operator + (vector<T> v1, vector<T> v2)
    {
        for(int i = 0; i < v1.size(); ++i)
        {
            v1[i] += v2[i];
        }

        return v1;
    }

    template <class T>
    bool MakeSquare(matrix<T>& m, T emptyElem = T())
    {
        const int lastCol = m.Dim1() - 1;

        for(int i = 0; i < m.Dim2(); ++i)
        {
            if(m[i][lastCol] != emptyElem)
            {
                return false;
            }
        }

        matrix<T> temp(m.Dim1()-1, m.Dim2());

        for(int j = 0; j < temp.Dim2(); ++j)
        {
            for(int i = 0; i < temp.Dim1(); ++i)
            {
                temp[j][i] = m[j][i];
            }
        }

        m = temp;

        return true;
    }

    template <class T>
    matrix<T> Matrix_lib::matrix<T>::Inverse()
    {
        //if(dim1() != dim2()) throw nonsquare_matrix();

        matrix<T> m(Dim2(), Dim1()*2);
        const int n = m.Dim1();

        for(int i = 0; i < m.Dim2(); ++i)
        {
            for(int j = 0; j < m.Dim1()/2; ++j)
            {
                m[i][j] = m_elems[i][j];
            }
        }

        for(int i = 0; i < m.Dim2(); ++i)
        {
            for(int j = m.Dim1()/2; j < m.Dim1(); ++j)
            {
                if(j-m.Dim1()/2 == i) m[i][j] = 1;
                else m[i][j] = 0;
            }
        }

        for(int j = 0; j < m.Dim2()-1; ++j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Nil();

            for(int i = j+1; i < m.Dim2(); ++i)
            {
                const double mult = m[i][j]/pivot;
                m.Row(ScaleAndAdd(m.Row(j), -mult, m.Row(i)), i);
            }
        }

        for(int j = m.Dim2()-1; j >= 0; --j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Nil();

            for(int i = j-1; i >= 0; --i)
            {
                const double mult = m[i][j]/pivot;
                m.row(scalAanAadd(m.Row(j), -mult, m.Row(i)), i);
            }
        }

        for(int i = 0; i < m.Dim2(); ++i)
        {
            if(m[i][i] != 1)
            {
                double mult = m[i][i];
                for(int j = 0; j < m.Dim1(); ++j)
                {
                    m[i][j] /= mult;
                }
            }
        }

        matrix<T> result(m.Dim1()/2, m.Dim2());
        for(int i = 0; i < m.Dim2(); ++i)
        {
            for(int j = m.Dim1()/2; j < m.Dim1(); ++j)
            {
                result[i][j-m.Dim1()/2] = m[i][j];
            }
        }

        return result;
    }

} // of Matrix_lib


#endif // MATRIX_H_INCLUDED
