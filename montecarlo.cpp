//
// Created by cniew on 7/16/25.
//

#include "montecarlo.h"
#include "./lib/Timer.h"
#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <immintrin.h>
#include <sleef.h>


// Function to generate normally distributed random numbers
double MonteCarlo::generateGaussianNoise(double mean, double stddev) {
    static std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(generator);
}

// Function to calculate the payoff of a European call option
double MonteCarlo::callOptionPayoff(double S, double K) {
    return std::max(S - K, 0.0);
}

// Function to calculate the payoff of a European put option
double MonteCarlo::putOptionPayoff(double S, double K) {
    return std::max(K - S, 0.0);
}

// Monte Carlo Simulation for European option pricing
double MonteCarlo::singleSim(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption) {
    double payoffSum = 0.0;
    for (int i = 0; i < numSimulations; ++i) {
        double ST = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * generateGaussianNoise(0.0, 1.0));
        payoffSum += isCallOption ? callOptionPayoff(ST, K) : putOptionPayoff(ST, K);
    }

    return payoffSum;
}

double MonteCarlo::priceOption(double S0, double K, double r, double sigma, double T,
                               int numSimulations, bool isCallOption) {
    unsigned int threads = std::thread::hardware_concurrency();
    std::vector<std::thread> workers;
    std::vector<double> partial_sums(threads, 0.0);  // avoid shared write
    int pathsPerThread = numSimulations / threads;
    int remainingPaths = numSimulations % threads;

    for (unsigned int i = 0; i < threads; ++i) {
        int localPaths = pathsPerThread + (i < remainingPaths ? 1 : 0);
        workers.emplace_back([&, i, localPaths]() {
            partial_sums[i] = singleSim(S0, K, r, sigma, T, localPaths, isCallOption);
        });
    }

    for (auto& t : workers) t.join();

    double total = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);
    return std::exp(-r * T) * (total / numSimulations);
}