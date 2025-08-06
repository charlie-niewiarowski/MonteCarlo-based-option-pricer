# Monte Carlo Options Pricing Engine
High-performance, SIMD-accelerated Monte Carlo simulation engine for pricing European call and put options. Optimized for speed and accuracy with AVX2 vectorization, multithreading, and real-time market data ingestion.

# Features
- **Integrates real-time data**,
  - fetching live stock prices from Alpaca and Yahoo Finance. Retrieves up-to-date risk-free rates from the U.S. Treasury API.

- **Monte Carlo Sim Pricing Class**
  - The constructor for this class takes in an option's
    - Initial stock price
    - Strick price
    - Risk-free interest rate
    - Standard deviation
    - Time to maturity
    - Number of simulations you want to run per pricing calculation
  - Utility functions
    - Functions for pricing call and put options (runs the number of simulations specified in the constructor)
    - Functions to get an average on the sims calculations (runs pricing call/put options N amount of times for a total of N * number of simulations)
  - Benchmarking functions
    - Standard error for calls and puts
    - Average time to calculate the price

# Project Structure
├── data.py               # Python script for data ingestion & CLI interface
├── cmake-build-release/  # C++ compiled binaries
├── montecarlo.h          # Monte Carlo pricer header
├── montecarlo.cpp        # SIMD-accelerated Monte Carlo implementation
├── main.cpp              # Entry point, argument parsing & result output
├── lib/
│   ├── Timer.h           # Lightweight benchmarking timer
│   └── Xoshiro256PlusSIMD/  # SIMD random number generator (Xoshiro256+)

## How to Use

**Prerequisites:** CMake, GCC/Clang with AVX2 support, Python3, pip

**Build**
- git clone https://github.com/yourusername/MonteCarloSim.git
- cd MonteCarloSim
- mkdir cmake-build-release
- cd cmake-build-release
- cmake ..
- make

**Running the Simulation**
python3 data.py

**Sample Output**
Call Statistics:
Average Time Per 1000000 Simulations: 10.52ms
Avg Call Price: 15.67
Standard Error: 0.0342

Put Statistics:
Average Time Per 1000000 Simulations: 10.76ms
Avg Put Price: 12.13
Standard Error: 0.0318

## License
MIT License

## References
Alpaca API Documentation

SLEEF Library
