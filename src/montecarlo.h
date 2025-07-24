//
// Created by cniew on 7/16/25.
//

#ifndef MONTECARLO_H
#define MONTECARLO_H
#define __AVX2_AVAILABLE__ 1

#include "../lib/Xoshiro256PlusSIMD/include/Xoshiro256Plus.h"
#include "../lib/Xoshiro256PlusSIMD/include/SIMDInstructionSet.h"

class MonteCarlo {
public:
    MonteCarlo(double S0, double K, double r, double sigma, double T, int numSimulations);
    double priceCallOption();
    double pricePutOption();

    double avgCallPrice(int N);
    double avgPutPrice(int N);

    // Benchmarking
    double stdErrorCall(int N);
    double stdErrorPut(int N);
    double avgCallTime(int N);
    double avgPutTime(int N);

private:
    double S0;    // Stock price
    double K;    // Strike price
    double r;     // Risk-free rate
    double sigma;  // Volatility
    double T;      // Time to maturity (1 year)
    int numSimulations; // Number of simulations

    SEFUtility::RNG::Xoshiro256Plus<SIMDInstructionSet::AVX2> rng;

    double totalCallPrice = 0;
    int callIterations = 0;
    double totalPutPrice = 0;
    int putIterations = 0;

    // Helpers ordered chronologically
    __m256d generateGaussianNoise(__m256d mean, __m256d stddev); // Gen random uniforms, use Box-Muller for random normals
    __m256d singleThreadCall(__m256d left, __m256d rightExponent, int pathsPerThread); // Brownian motion call payoff
    __m256d singleThreadPut(__m256d left, __m256d rightExponent, int pathsPerThread); // Brownian motion put payoff
};



#endif //MONTECARLO_H
