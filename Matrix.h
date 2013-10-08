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
            matrix(int ii, int jj) : nrow(ii), ncol(jj)
            {
                vector<T> empty_row;
                for(int i = 0; i < ncol; ++i)
                    empty_row.push_back(T());
                for(int j = 0; j < nrow; ++j)
                {
                    elems.push_back(empty_row);
                }
            }

            int dim1() const { return ncol; } // количество элементов в строке
            int dim2() const { return nrow; } // количество строк

            vector<T>& operator[](int n)
            {
                if(n >= nrow) throw IndexingError();
                return elems[n];
            }

            void row(vector<T> v, int ind)
            {
                if(v.size() > ncol || ind >= nrow)
                    throw IndexingError();

                elems[ind] = v;
            }
            vector<T> row(int ind)
            {
                if(ind >= nrow)
                    throw IndexingError();
                return elems[ind];
            }

            void swap_rows(int f, int s)
            {
                vector<T> temp;
                temp = row(f);
                row(row(s), f);
                row(temp, s);
            }

            matrix<T>& operator =(matrix<T> m)
            {
                elems.resize(m.dim2());
                ncol = m.dim1();
                nrow = m.dim2();

                for(int i = 0; i < nrow; ++i)
                {
                    elems[i].resize(m.dim1());
                }

                for(int j = 0; j < m.dim2(); ++j)
                {
                    for(int i = 0; i < m.dim1(); ++i)
                    {
                        elems[j][i] = m[j][i];
                    }
                }

                return *this;
            }

            matrix<T> operator *(matrix<T> m)
            {
                matrix<T> result(m.dim1(), dim2());

                for(int i = 0; i < dim2(); ++i)
                {
                    for(int j = 0; j < m.dim1(); ++j)
                    {
                        for(int k = 0; k < dim1(); ++k)
                        {
                            result[i][j] += elems[i][k]*m[k][j];
                        }
                    }
                }

                return result;
            }

            matrix<T> Inverse();

        private:
            int nrow;
            int ncol;

            vector< vector<T> > elems;

            friend bool MakeSquare<>(matrix<T>&, T);
    };

//---------------------------------------------------------

    template<class T>
    ostream& operator << (ostream& stream, matrix<T> m)
    {
        for(int i = 0; i < m.dim2(); ++i)
        {
            for(int j = 0; j < m.dim1(); ++j)
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
        for(int j = 0; j < m.dim2(); ++j)
        {
            for(int i = 0; i < m.dim1(); ++i)
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
        for(int i = 0; i < v1.size(); ++i) sum += v1[i]*v2[i];

        return sum;
    }

    template<class T>
    vector<T> operator * (matrix<T> m, vector<T> v)
    {
        vector<T> result(m.dim2());

        for(int i = 0; i < m.dim2(); ++i)
            result[i] = dot_product(m.row(i), v);

        return result;
    }

    template<class T>
    vector<T> operator - (vector<T> v1, vector<T> v2)
    {
        for(int i = 0; i < v1.size(); ++i) v1[i] -= v2[i];

        return v1;
    }

    template<class T>
    vector<T> operator + (vector<T> v1, vector<T> v2)
    {
        for(int i = 0; i < v1.size(); ++i) v1[i] += v2[i];

        return v1;
    }

    template <class T>
    bool MakeSquare(matrix<T>& m, T empty_elem = T())
    {
        const int last_col = m.dim1() - 1;

        for(int i = 0; i < m.dim2(); ++i)
        {
            if(m[i][last_col] != empty_elem) return false;
        }

        matrix<T> temp(m.dim1()-1, m.dim2());

        for(int j = 0; j < temp.dim2(); ++j)
        {
            for(int i = 0; i < temp.dim1(); ++i)
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

        matrix<T> m(dim2(), dim1()*2);
        const int n = m.dim1();

        for(int i = 0; i < m.dim2(); ++i)
        {
            for(int j = 0; j < m.dim1()/2; ++j)
            {
                m[i][j] = elems[i][j];
            }
        }

        for(int i = 0; i < m.dim2(); ++i)
        {
            for(int j = m.dim1()/2; j < m.dim1(); ++j)
            {
                if(j-m.dim1()/2 == i) m[i][j] = 1;
                else m[i][j] = 0;
            }
        }

        for(int j = 0; j < m.dim2()-1; ++j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Nil();

            for(int i = j+1; i < m.dim2(); ++i)
            {
                const double mult = m[i][j]/pivot;
                m.row(scale_and_add(m.row(j), -mult, m.row(i)), i);
            }
        }

        for(int j = m.dim2()-1; j >= 0; --j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Nil();

            for(int i = j-1; i >= 0; --i)
            {
                const double mult = m[i][j]/pivot;
                m.row(scale_and_add(m.row(j), -mult, m.row(i)), i);
            }
        }

        for(int i = 0; i < m.dim2(); ++i)
        {
            if(m[i][i] != 1)
            {
                double mult = m[i][i];
                for(int j = 0; j < m.dim1(); ++j)
                {
                    m[i][j] /= mult;
                }
            }
        }

        matrix<T> result(m.dim1()/2, m.dim2());
        for(int i = 0; i < m.dim2(); ++i)
        {
            for(int j = m.dim1()/2; j < m.dim1(); ++j)
            {
                result[i][j-m.dim1()/2] = m[i][j];
            }
        }

        return result;
    }

} // of Matrix_lib


#endif // MATRIX_H_INCLUDED
