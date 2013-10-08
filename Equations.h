#ifndef EQUATIONS_H_INCLUDED
#define EQUATIONS_H_INCLUDED

#include "Matrix.h"
#include <numeric>
#include <cmath>

//---------------------------------------------------------

namespace Equations
{
    typedef Matrix_lib::matrix<double> Matrix;
    typedef std::vector<double> Vector;

    Vector GaussianElimination(Matrix m, Vector vec)
    {
        if(m.dim1() != m.dim2()) throw Matrix_lib::NonsquareMatrix();
        if(m.dim2() != vec.size()) throw Matrix_lib::IncorrectSystem();
        const int n = m.dim1();

        for(int j = 0; j < n-1; ++j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            for(int i = j+1; i < n; ++i)
            {
                const double mult = m[j][i]/pivot;
                m.row(Matrix_lib::ScaleAndAdd(m.row(j), -mult, m.row(i)), i);
                vec[i] -= mult * vec[j];
            }
        }

        for(int j = n-1; j >= 0; --j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            for(int i = j-1; i >= 0; --i)
            {
                const double mult = m[j][i]/pivot;
                m.row(Matrix_lib::ScaleAndAdd(m.row(j), -mult, m.row(i)), i);
                vec[i] -= mult * vec[j];
            }
        }

        for(int j = 0 ; j < n; ++j)
        {
            vec[j] /= m[j][j];
        }
        return vec;
    }

//---------------------------------------------------------

    Vector ModernGaussianElimination(Matrix m, Vector vec)
    {
        if(m.dim1() != m.dim2()) throw Matrix_lib::NonsquareMatrix();
        if(m.dim2() != vec.size()) throw Matrix_lib::IncorrectSystem();
        const int n = m.dim1();

        // проходим от первого столбца до последнего,
        // обнул€€ элементы ниже главной диагонали

        for(int j = 0; j < n-1; ++j)
        {
            int pivot_row = j;

            for(int k = j+1; k < n; ++k)
                if(abs(m[j][k]) > abs(m[j][pivot_row]))
                    pivot_row = k;

            if(pivot_row != j)
            {
                m.swap_rows(j, pivot_row);
                swap(vec[j], vec[pivot_row]);
            }

            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            // обнул€ем элементы, сто€щие ниже диагонали в строке i
            for(int i = j+1; i < n; ++i)
            {
                const double mult = m[j][i]/pivot;
                m.row(Matrix_lib::ScaleAndAdd(m.row(j), -mult, m.row(i)), i);
                vec[i] -= mult * vec[j];
            }
        }

        // проходим от первого столбца до последнего,
        // обнул€€ элементы выше главной диагонали

        for(int j = n-1; j >= 0; --j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            // обнул€ем элементы, сто€щие выше диагонали в строке i
            for(int i = j-1; i >= 0; --i)
            {
                const double mult = m[j][i]/pivot;
                m.row(Matrix_lib::ScaleAndAdd(m.row(j), -mult, m.row(i)), i);
                vec[i] -= mult * vec[j];
            }
        }

        for(int j = 0 ; j < n; ++j)
        {
            vec[j] /= m[j][j];
        }
        return vec;
    }

    /*int count_of_iter(Matrix C, Vector b, double precision_of_calc = 0.001)
    {
        double max_C = 0;
        double temp_max = 0;
        double max_b = 0;
        double sum = 0;
        Vector current_line;
        for(int i = 0; i < C.dim2(); ++i, sum = 0)
        {
            current_line = C.row(i);
            sum = accumulate(current_line.begin(), current_line.end(), sum);
            temp_max = abs(sum);
            if(temp_max > max_C) max_C = temp_max;
        }

        max_b = *(max_element(b.begin(), b.end()));

        int N = ( log( (precision_of_calc*(1 - max_C))/max_b ) )/log(max_C) + 1;
        cout << "N: " << N << endl;

        return N;
    }

    Vector Seidel_method(Matrix A, Vector b)
    {
        if(A.dim1() != A.dim2()) throw Matrix_lib::nonsquare_matrix();

        for(int j = 0; j < A.dim2(); ++j)
        {
            const double pivot = A[j][j];

            for(int i = 0; i < A.dim1(); ++i)
            {
                A[i][j] /= pivot;
            }

            b[j] /= pivot;
            A[j][j] = 0;
        }

        Vector X(b.size());

        int N = count_of_iter(A, b);

        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < A.dim1(); ++j)
            {
                X[j] = b[j] - Matrix_lib::dot_product(A.row(j), X);
            }
        }

        return X;
    }*/
} // of Equations

#endif // EQUATIONS_H_INCLUDED
