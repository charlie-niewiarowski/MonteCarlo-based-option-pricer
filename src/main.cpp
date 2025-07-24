#include <iostream>

#include "montecarlo.h"
#include "../lib/Timer.h"

int main() {
    // Option parameters
    double S0 = 213.76;   // Initial stock price
    double K = 100.0;    // Strike price
    double r = 0.04306;     // Risk-free rate
    double sigma = 0.3172;  // Volatility
    double T = 1;      // Time to maturity (1 year)
    int numSimulations = 100000000; // Number of simulations

    // Calculate option prices
    double callPrice = 0;
    double putPrice = 0;
    MonteCarlo mc(S0, K, r, sigma, T, numSimulations);

    // Output the results
    std::cout << "Call Statistics: " << std::endl;
    std::cout << "Average Time Per " << numSimulations << " Simulations: " << mc.avgCallTime(250) << "ms" << std::endl;
    std::cout << "Avg Call Price: " << mc.avgCallPrice(250) << std::endl;
    std::cout << "Standard Error: " << mc.stdErrorCall(250) << std::endl;

    std::cout << "Put Statistics: " << std::endl;
    std::cout << "Average Time Per " << numSimulations << " Simulations: " << mc.avgPutTime(250) << "ms" << std::endl;
    std::cout << "Avg Put Price: " << mc.avgPutPrice(250) << std::endl;
    std::cout << "Standard Error: " << mc.stdErrorPut(250) << std::endl;
    
    return 0;
}