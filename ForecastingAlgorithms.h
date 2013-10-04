#ifndef FORECASTINGALGORITHMS_H_INCLUDED
#define FORECASTINGALGORITHMS_H_INCLUDED

#include <vector>
#include <iostream>

namespace ForecastingAlgorithms
{
    typedef std::vector<double> Prediction;
    typedef double Value;

    class ForecastingAlgorithm
    {
        public:
            virtual Prediction DoPrediction(Value value, Prediction history) = 0;
            virtual ~ForecastingAlgorithm() {};
        protected:
            ForecastingAlgorithm() {};
    };

    class MovingAverage : public ForecastingAlgorithm
    {
        public:
            MovingAverage(int n);
            Prediction DoPrediction(Value value, Prediction history);
    };

} // of ForecastingAlgorithms

#endif // FORECASTINGALGORITHMS_H_INCLUDED
