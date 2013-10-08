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
    typedef Matrix_lib::matrix<double> Matrix;
    typedef std::vector<double> Vector;

    using Std_Deviation::StdDeviation;
    using Equations::GaussianElimination;

    ///-----------------------------------------------------------------
    /// base class interface
    ///-----------------------------------------------------------------

    class ForecastingAlgorithm
    {
        public:
            virtual Value DoPrediction(Value value, History history) = 0;
            virtual ~ForecastingAlgorithm() {};
            virtual std::string GetName() = 0;
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
            std::string GetName() { return std::string("Moving Average"); }
        private:
            int m_base;
    };

    //-----------------------------------------------------------------

    class ExponentialSmoothing : public ForecastingAlgorithm
    {
        public:
            ExponentialSmoothing(const double a);
            Value DoPrediction(Value value, History history);
            std::string GetName() { return std::string("Exponential Smoothing"); }
        private:
            double m_base;
    };

    //-----------------------------------------------------------------

    class RegressionAnalysis : public ForecastingAlgorithm
    {
        public:
            RegressionAnalysis();
            Value DoPrediction(Value value, History history);
            std::string GetName() { return std::string("Regression Analysis"); }
    };

    //-----------------------------------------------------------------

    class AutoregressiveModel : public ForecastingAlgorithm
    {
        public:
            AutoregressiveModel(int sft);
            Value DoPrediction(Value value, History history);
            std::string GetName() { return std::string("Autoregressive Model"); }
        private:
            Vector Slice(Vector v1, int start);
            Vector CalculateAutocorrelation();
            Value CalculateCovariance(Vector v1, Vector v2);

            Vector m_v;
            int m_shift;
            int m_size;
            Matrix m_correlationMatrix;
            Vector m_correlations;
    };

    ///-----------------------------------------------------------------
    /// exception class implementations
    ///-----------------------------------------------------------------

    class IncorrectHistoryException
    {
        public:
            IncorrectHistoryException(std::string msg) : m_message(msg) {};
            std::string GetMessage() { return m_message; };
        private:
            std::string m_message;
    };

} // of ForecastingAlgorithms

#endif // FORECASTINGALGORITHMS_H_INCLUDED
