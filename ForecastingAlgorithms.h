#ifndef FORECASTINGALGORITHMS_H_INCLUDED
#define FORECASTINGALGORITHMS_H_INCLUDED

#include <vector>
#include <iostream>
#include <cmath>
#include "Std_Deviation.h"
#include "Equations.h"
#include "Matrix.h"

#define DONOTHING()

namespace ForecastingAlgorithms
{
    typedef std::vector<double> History;
    typedef double Value;

    ///-----------------------------------------------------------------
    /// base class interface
    ///-----------------------------------------------------------------

    class ForecastingAlgorithm
    {
        public:
            virtual Value DoPrediction(Value value, History history) = 0;
            virtual ~ForecastingAlgorithm() {};
        protected:
            ForecastingAlgorithm() {};
    };

    ///-----------------------------------------------------------------
    /// main class interfaces
    ///-----------------------------------------------------------------

    class MovingAverage : public ForecastingAlgorithm
    {
        public:
            MovingAverage(int n);
            Value DoPrediction(Value value, History history);

            inline int GetBase() { return base; }
        private:
            int base;
    };

    //-----------------------------------------------------------------

    class ExponentialSmoothing : public ForecastingAlgorithm
    {
        public:
            ExponentialSmoothing(const double a);
            Value DoPrediction(Value value, History history);
            double GetBase() { return base; }
        private:
            double base;
    };

    //-----------------------------------------------------------------

    class RegressionAnalysis : public ForecastingAlgorithm
    {
        public:
            RegressionAnalysis();
            Value DoPrediction(Value value, History history);
    };

    //-----------------------------------------------------------------

    class AutoregressiveModel : public ForecastingAlgorithm
    {
        public:
            AutoregressiveModel(int sft);
        private:
            Vector v;
            int shift;
            int size;
            Matrix CorrelationMatrix;
            Vector Correlations;
    };

    //-----------------------------------------------------------------

    ///-----------------------------------------------------------------
    /// exception class implementations
    ///-----------------------------------------------------------------

    class IncorrectHistoryException
    {
        public:
            IncorrectHistoryException(std::string msg) : message(msg) {};
            std::string GetMessage() { return message; };
        private:
            std::string message;
    };

} // of ForecastingAlgorithms

#endif // FORECASTINGALGORITHMS_H_INCLUDED
