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
        if(m.Dim1() != m.Dim2()) throw Matrix_lib::NonsquareMatrix();
        if(m.Dim2() != vec.size()) throw Matrix_lib::IncorrectSystem();
        const int n = m.Dim1();

        for(int j = 0; j < n-1; ++j)
        {
            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            for(int i = j+1; i < n; ++i)
            {
                const double mult = m[j][i]/pivot;
                m.Row(Matrix_lib::ScaleAndAdd(m.Row(j), -mult, m.Row(i)), i);
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
                m.Row(Matrix_lib::ScaleAndAdd(m.Row(j), -mult, m.Row(i)), i);
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
        if(m.Dim1() != m.Dim2()) throw Matrix_lib::NonsquareMatrix();
        if(m.Dim2() != vec.size()) throw Matrix_lib::IncorrectSystem();
        const int n = m.Dim1();

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
                m.SwapRows(j, pivot_row);
                swap(vec[j], vec[pivot_row]);
            }

            const double pivot  = m[j][j];
            if(pivot == 0) throw Matrix_lib::Nil();

            // обнул€ем элементы, сто€щие ниже диагонали в строке i
            for(int i = j+1; i < n; ++i)
            {
                const double mult = m[j][i]/pivot;
                m.Row(Matrix_lib::ScaleAndAdd(m.Row(j), -mult, m.Row(i)), i);
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
                m.Row(Matrix_lib::ScaleAndAdd(m.Row(j), -mult, m.Row(i)), i);
                vec[i] -= mult * vec[j];
            }
        }

        for(int j = 0 ; j < n; ++j)
        {
            vec[j] /= m[j][j];
        }
        return vec;
    }
} // of Equations

#endif // EQUATIONS_H_INCLUDED
