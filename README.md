Monte Carlo Options Pricing Engine
High-performance, SIMD-accelerated Monte Carlo simulation engine for pricing European call and put options. Optimized for speed and accuracy with AVX2 vectorization, multithreading, and real-time market data ingestion.

Features
Real-Time Market Data Integration

Fetches live stock prices from Alpaca and Yahoo Finance.

Retrieves up-to-date risk-free rates from the U.S. Treasury API.

SIMD-Optimized Pricing Engine

Utilizes AVX2 intrinsics for vectorized Monte Carlo paths.

Custom Box-Muller Gaussian generator with SIMD support.

Multithreaded Execution

Dynamically scales to hardware concurrency.

Cache-friendly data structures for parallel execution.

Benchmarking and Error Metrics

Compute average option prices across N iterations.

Tracks standard errors and per-iteration latencies.

Seamless Python Integration

Python script (data.py) automates market data retrieval and interfaces with the C++ engine.

Project Structure
bash
Copy
Edit
.
├── data.py               # Python script for data ingestion & CLI interface
├── cmake-build-release/  # C++ compiled binaries
├── montecarlo.h          # Monte Carlo pricer header
├── montecarlo.cpp        # SIMD-accelerated Monte Carlo implementation
├── main.cpp              # Entry point, argument parsing & result output
├── lib/
│   ├── Timer.h           # Lightweight benchmarking timer
│   └── Xoshiro256PlusSIMD/  # SIMD random number generator (Xoshiro256+)
Why You Should Use This
At high-frequency trading firms, performance is everything. This engine was built with quantitative development in mind, optimized down to the instruction level:

Ultra-fast simulation: Leveraging AVX2 SIMD intrinsics and multithreading to process billions of paths within milliseconds.

Low-level systems optimization: Implements custom SIMD math routines (Box-Muller transform, exponential/logarithms) to bypass typical library bottlenecks.

Practical for real traders: Integrated live data pipelines (Alpaca, Yahoo Finance, US Treasury) to ensure this is built for real market conditions, not toy models.

Accuracy and statistical rigor: Benchmarking modules calculate error margins and per-simulation latency, ensuring statistical convergence isn’t sacrificed for speed.

The author has applied similar systems-level optimizations across projects like a custom HTTP server in C, Python-based web scrapers processing over 30,000 items in seconds, and Neo4j-based real-time balance tracking systems. These experiences culminate in an engineering mindset focused on performance, modularity, and practical deployment.

Build Instructions
bash
Copy
Edit
# Prerequisites: CMake, GCC/Clang with AVX2 support, Python3, pip
git clone https://github.com/yourusername/MonteCarloSim.git
cd MonteCarloSim
mkdir cmake-build-release
cd cmake-build-release
cmake ..
make
Running the Simulation
bash
Copy
Edit
python3 data.py
# You will be prompted for:
# Symbol, Strike, Time to Maturity, Number of Simulations
Sample Output
yaml
Copy
Edit
Call Statistics:
Average Time Per 1000000 Simulations: 10.52ms
Avg Call Price: 15.67
Standard Error: 0.0342

Put Statistics:
Average Time Per 1000000 Simulations: 10.76ms
Avg Put Price: 12.13
Standard Error: 0.0318
Future Work
AVX-512 support for newer architectures.

Custom SIMD math library to replace external dependencies.

Extend to American options using Longstaff-Schwartz algorithm.

Python C-extension interface for seamless integration in quant research workflows.

License
MIT License

References
Alpaca API Documentation

SLEEF Library
