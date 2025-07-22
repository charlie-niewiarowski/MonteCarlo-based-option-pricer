//
// Created by cniew on 7/16/25.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <immintrin.h>

class MonteCarlo {
public:
    __m256d singleThreadSim(__m256d left, __m256d rightExponent, __m256d K, int numSims, bool isCallOption);
    double priceOption(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption);
private:
    void boxMullerAVX(__m256d u1, __m256d u2, __m256d& z0, __m256d& z1);
    __m256d generateGaussianNoise(__m256d mean, __m256d stddev);
    __m256d callOptionPayoff(__m256d S, __m256d K);
    __m256d putOptionPayoff(__m256d S, __m256d K);
};



#endif //MONTECARLO_H
