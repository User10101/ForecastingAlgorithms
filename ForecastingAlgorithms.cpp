#include "ForecastingAlgorithms.h"

using namespace std;
using namespace ForecastingAlgorithms;

MovingAverage::MovingAverage(int n)
{
    cout << n << endl;
}

Prediction MovingAverage::DoPrediction(Value value, Prediction history)
{
    cout << "Yes!" << endl;
}

int main()
{
    MovingAverage mv(5);
    return 0;
}
