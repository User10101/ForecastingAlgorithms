#include "ForecastingAlgorithms.h"

using namespace std;
using namespace ForecastingAlgorithms;

///-----------------------------------------------------------------
/// Moving Average Implementation
///-----------------------------------------------------------------

MovingAverage::MovingAverage(const int n) : base(n)
{
    DONOTHING();
}

//-----------------------------------------------------------------

Value MovingAverage::DoPrediction(Value value, History history)
{
    Value result = 0;
    int t = history.size();
    int base = GetBase();
    if( t < base )
    {
        throw IncorrectHistoryException("base bigger then history size.");
    }

    for(int i = 0; i < base; i++)
    {
        result += history[t - base + i];
    }

    result /= base;

    return result;
}

///-----------------------------------------------------------------
/// Exponential Smoothing Implementation
///-----------------------------------------------------------------

ExponentialSmoothing::ExponentialSmoothing(const double a) : base(a)
{
    DONOTHING()
}

//-----------------------------------------------------------------

Value ExponentialSmoothing::DoPrediction(Value value, History history)
{
    int t = history.size()-1;
    double a = GetBase();
    Value result = a*value;

    while(t != 0)
    {
        result += a*pow(1-a, history.size()-t)*history[t];
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
/// main function
///-----------------------------------------------------------------

int main()
try{
    MovingAverage mv(4);
    ExponentialSmoothing es(0.3);
    RegressionAnalysis ra;
    History p(4);
    p[0] = 2;
    p[1] = 3;
    p[2] = 2;
    p[3] = 5;

    cout << mv.DoPrediction(4, p) << endl;
    cout << es.DoPrediction(4, p) << endl;
    cout << ra.DoPrediction(4, p) << endl;

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
