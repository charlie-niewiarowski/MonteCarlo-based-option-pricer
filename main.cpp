#include <iostream>

#include "montecarlo.h"
#include "./lib/Timer.h"

int main() {
    // Option parameters
    double S0 = 100.0;   // Initial stock price
    double K = 100.0;    // Strike price
    double r = 0.05;     // Risk-free rate
    double sigma = 0.2;  // Volatility
    double T = 1;      // Time to maturity (1 year)
    int numSimulations = 1000000; // Number of simulations

    // Calculate option prices
    double callPrice = 0;
    double putPrice = 0;
    {
        MonteCarlo mc;
        Timer timer;
        callPrice = mc.priceOption(S0, K, r, sigma, T, numSimulations, true);
        putPrice = mc.priceOption(S0, K, r, sigma, T, numSimulations, false);
    }

    // Output the results
    std::cout << "European Call Option Price: " << callPrice << std::endl;
    std::cout << "European Put Option Price: " << putPrice << std::endl;

    return 0;
}