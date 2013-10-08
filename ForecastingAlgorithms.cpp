#include "ForecastingAlgorithms.h"

using namespace std;
using namespace ForecastingAlgorithms;

using namespace Matrix_lib;

///-----------------------------------------------------------------
/// Moving Average Implementation
///-----------------------------------------------------------------

MovingAverage::MovingAverage(const int n) : m_base(n)
{
    DONOTHING();
}

//-----------------------------------------------------------------

Value MovingAverage::DoPrediction(Value value, History history)
{
    Value result = 0;
    int t = history.size();
    if( t < m_base )
    {
        throw IncorrectHistoryException("base bigger then history size.");
    }

    for(int i = 0; i < m_base; i++)
    {
        result += history[t - m_base + i];
    }

    result /= m_base;

    return result;
}

///-----------------------------------------------------------------
/// Exponential Smoothing Implementation
///-----------------------------------------------------------------

ExponentialSmoothing::ExponentialSmoothing(const double a) : m_base(a)
{
    DONOTHING()
}

//-----------------------------------------------------------------

Value ExponentialSmoothing::DoPrediction(Value value, History history)
{
    int t = history.size()-1;
    Value result = m_base*value;

    while(t != 0)
    {
        result += m_base*pow(1-m_base, history.size()-t)*history[t];
        t--;
    }

    return result;
}

///-----------------------------------------------------------------
/// Regression Analysis Implementation
///-----------------------------------------------------------------

RegressionAnalysis::RegressionAnalysis()
{
    DONOTHING()
}

//-----------------------------------------------------------------

Value RegressionAnalysis::DoPrediction(Value value, History history)
{
    double sumX, sumY, sumXY, sumX2, sumY2;
    double a, b;
    double _x, _y;
    const int n = history.size();
    int t = history.size();

    sumX = sumY = sumXY = sumX2 = sumY2 = 0;

    for(int i = 0; i < n; ++i)
    {
        sumX += i;
        sumY += history[i];
        sumXY += i*history[i];
        sumX2 += i*i;
        sumY2 += history[i]*history[i];
    }

    _x = sumX/n;
    _y = sumY/n;

    b = (sumXY- n*_x*_y) / (sumX2 - n*_x*_x);
    a = _y - b*_x;

    double result = a + b*t;
    return result;
}

///-----------------------------------------------------------------
/// Autoregressive Model Implementation
///-----------------------------------------------------------------

AutoregressiveModel::AutoregressiveModel(int sft)
    : m_shift(sft), m_correlationMatrix(0, 0)
{
    DONOTHING();
}

//-----------------------------------------------------------------

Vector AutoregressiveModel::Slice(Vector v1, int start)
{
    Vector v2(m_shift);

    for(int i = 0; i < m_shift; ++i)
    {
        v2[i] = v1[start+i];
    }

    return v2;
}

//-----------------------------------------------------------------

Vector AutoregressiveModel::CalculateAutocorrelation()
{
    int current = m_shift;
    Vector Covariance(m_size);
    Vector Correlation(m_size);
    Vector v1(m_shift), v2(m_shift);

    v1 = Slice(m_v, 0);
    StdDeviation sv1(v1);

    for(int i = 0; i < m_size; ++i)
    {
        v2 = Slice(m_v, i);

        StdDeviation sv2(v2);

        Covariance[i] = CalculateCovariance(v1, v2);
        Correlation[i] = Covariance[i] / (sv1.CalculateStandardDeviation()
                                          * sv2.CalculateStandardDeviation());
    }

    return Correlation;
}

//-----------------------------------------------------------------

double AutoregressiveModel::CalculateCovariance(Vector v1, Vector v2)
{
    StdDeviation sv1(v1);
    StdDeviation sv2(v2);

    double Mx = sv1.M();
    double My = sv2.M();

    double total = 0;
    for(int i = 0; i < m_shift; ++i)
    {
        total += (v1[i] - Mx)*(v2[i] - My);
    }


    return total / m_shift;
}

//-----------------------------------------------------------------

Value AutoregressiveModel::DoPrediction(Value value, History history)
{
    history.push_back(value);
    m_v = history;
    m_size = history.size()-m_shift+1;

    Matrix tmp(m_size-1, m_size-1);
    m_correlationMatrix = tmp;

    m_correlations = CalculateAutocorrelation();

    //cout << CorrelationMatrix << endl;

    for(int i = 0; i < m_size-1; ++i)
    {
        for(int j = 0; j < m_size-1; ++j)
        {
            if(i == 0)
            {
                m_correlationMatrix[i][j] = m_correlations[j];
            }
            else
            {
                if(j == 0)
                    m_correlationMatrix[i][j] = m_correlations[i];
                else
                    m_correlationMatrix[i][j] = m_correlationMatrix[i-1][j-1];
            }
        }
    }

    Vector A(m_size-1);
    Vector startSolution(m_size-1);

    for(int i = 1; i < m_size; ++i)
        startSolution[i-1] = m_correlations[i];

    A = GaussianElimination(m_correlationMatrix, startSolution);

    double result = 0;
    for(int i = 0, t = m_size-2; i < m_size-1; i++)
    {
        result += m_v[m_v.size()-1-i]*A[i];
    }

    return result;
}

///-----------------------------------------------------------------
/// main function
///-----------------------------------------------------------------

int main()
try{
    ForecastingAlgorithm* fa[4];
    fa[0] = new MovingAverage(3);
    fa[1] = new ExponentialSmoothing(0.3);
    fa[2] = new RegressionAnalysis;
    fa[3] = new AutoregressiveModel(3);

    History p(4);
    p[0] = 2;
    p[1] = 10;
    p[2] = 2;
    p[3] = 9;

    for(int i = 0; i < 4; i++)
    {
        cout << fa[i]->GetName() << " : " << fa[i]->DoPrediction(2, p) << endl;
    }

    return 0;
}

///-----------------------------------------------------------------
/// exception handlers
///-----------------------------------------------------------------

catch(IncorrectHistoryException e)
{
    std::cerr << "Error: " << e.GetMessage() << endl;
    return 1;
}
