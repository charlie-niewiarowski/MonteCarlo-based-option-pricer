#include <iostream>

#include "montecarlo.h"
#include "../lib/Timer.h"

int main(int argc, char* argv[]) {
    std::cout << argc << std::endl;
    // Option parameters
    if (argc != 7) { // Program name + 6 args expected
        std::cerr << "Usage: " << argv[0] << " S0 K r sigma T numSimulations\n";
        return 1;
    }
    double S0; double K; double r; double sigma; double T; int numSimulations;
    try {
        S0 = std::stod(argv[1]);              // Initial stock price
        K = std::stod(argv[2]);               // Strike price
        r = std::stod(argv[3]);               // Risk-free rate
        sigma = std::stod(argv[4]);           // Volatility
        T = std::stod(argv[5]);               // Time to maturity
        numSimulations = std::stoi(argv[6]);    // Number of simulations


        // Now you can use these variables in your Monte Carlo pricer...

    } catch (const std::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << "\n";
        return 1;
    }


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