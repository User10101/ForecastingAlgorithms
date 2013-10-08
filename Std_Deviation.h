#ifndef STD_DEVIATION_H_INCLUDED
#define STD_DEVIATION_H_INCLUDED

#include <vector>

namespace Std_Deviation
{
    class StdDeviation
    {
        private:
            int max;
            std::vector<double> value;
            double mean;
        public:
            StdDeviation(std::vector<double> vec)
            {
                value = vec;
                max = vec.size();
            }

            double M()
            {
                double sum = 0;

                for(int i = 0; i < max; ++i)
                {
                    sum += value[i];
                }

                return sum / max;
            }

            double D()
            {
                mean = M();
                double temp = 0;

                for(int i = 0; i < max; ++i)
                {
                    temp += (value[i] - mean) * (value[i] - mean);
                }

                return temp / max;
            }

            double CalculateStandardDeviation()
            {
                return sqrt(D());
            }
    };
} // of Std_Deviation

#endif // STD_DEVIATION_H_INCLUDED
