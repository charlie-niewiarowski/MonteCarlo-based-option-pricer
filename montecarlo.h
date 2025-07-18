//
// Created by cniew on 7/16/25.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H


class MonteCarlo {
public:
    double singleSim(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption);
    double priceOption(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption);
private:
    double generateGaussianNoise(double mean, double stddev);
    double callOptionPayoff(double S, double K);
    double putOptionPayoff(double S, double K);
};



#endif //MONTECARLO_H
