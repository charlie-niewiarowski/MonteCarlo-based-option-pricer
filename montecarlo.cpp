//
// Created by cniew on 7/16/25.
//

#include "montecarlo.h"
#include "./lib/Timer.h"
#include <iostream>
#include <cmath>
#include <random>
#include <thread>

namespace sleef {
    extern "C++" {
        #include <sleef.h>
    }
}


void MonteCarlo::boxMullerAVX(__m256d u1, __m256d u2, __m256d& z0, __m256d& z1) {
    const __m256d epsilon = _mm256_set1_pd(1e-10);
    const __m256d minusTwo = _mm256_set1_pd(-2.0);
    const __m256d twoPi = _mm256_set1_pd(2.0 * M_PI);

    u1 = _mm256_max_pd(u1, epsilon);  // Avoid log(0)

    __m256d r = sleef::Sleef_sqrtd4_avx2(_mm256_mul_pd(minusTwo, sleef::Sleef_logd4_u10avx2(u1)));
    __m256d theta = _mm256_mul_pd(twoPi, u2);

    z0 = _mm256_mul_pd(r, sleef::Sleef_cosd4_u10avx2(theta));
    z1 = _mm256_mul_pd(r, sleef::Sleef_sind4_u10avx2(theta));
}

__m256d MonteCarlo::generateGaussianNoise(__m256d mean, __m256d stddev) {
    thread_local static std::mt19937_64 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    alignas(32) double u1_arr[4];
    alignas(32) double u2_arr[4];

    for (int i = 0; i < 4; ++i) {
        u1_arr[i] = std::max(distribution(generator), 1e-10); // avoid log(0)
        u2_arr[i] = distribution(generator);
    }

    __m256d u1 = _mm256_load_pd(u1_arr);
    __m256d u2 = _mm256_load_pd(u2_arr);

    __m256d z0, z1;
    boxMullerAVX(u1, u2, z0, z1);

    // Return 4 normals scaled to mean/stddev
    return _mm256_add_pd(_mm256_mul_pd(z0, stddev), mean);
}

__m256d MonteCarlo::callOptionPayoff(__m256d S, __m256d K) {
    return _mm256_max_pd(_mm256_sub_pd(S, K), _mm256_set1_pd(0.0));
}

__m256d MonteCarlo::putOptionPayoff(__m256d S, __m256d K) {
    return _mm256_max_pd(_mm256_sub_pd(K, S), _mm256_set1_pd(0.0));
}

__m256d MonteCarlo::singleThreadSim(__m256d left, __m256d rightExponent, __m256d K, int numSims, bool isCallOption) {
    __m256d payoffSum = _mm256_setzero_pd();
    for (int i = 0; i < numSims; i += 4) {
        __m256d Z = generateGaussianNoise(_mm256_set1_pd(0.0), _mm256_set1_pd(1.0));
        __m256d ST = _mm256_mul_pd(left, sleef::Sleef_expd4_u10avx2(_mm256_mul_pd(rightExponent, Z)));

        __m256d payoff = isCallOption ? callOptionPayoff(ST, K) : putOptionPayoff(ST, K);
        payoffSum = _mm256_add_pd(payoffSum, payoff);
    }

    return payoffSum;
}

double MonteCarlo::priceOption(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption) {
    int threads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> workers;
    std::vector<double> partial_sums(threads, 0.0);  // avoid shared write
    int pathsPerThread = numSimulations / threads;

    __m256d left = _mm256_set1_pd(S0 * std::exp((r - 0.5 * sigma * sigma) * T));
    __m256d rightExponent = _mm256_set1_pd(sigma * std::sqrt(T));

    std::vector<double> threadSums(threads, 0.0);
    for (unsigned int i = 0; i < threads; ++i) {
        workers.emplace_back([&, i, pathsPerThread]() {
            __m256d vectorizedTotal = singleThreadSim(left, rightExponent, _mm256_set1_pd(K), pathsPerThread, isCallOption);
            double vec[4];
            _mm256_storeu_pd(vec, vectorizedTotal);
            threadSums[i] = vec[0] + vec[1] + vec[2] + vec[3];
        });
    }

    for (auto& t : workers) t.join();

    double total = std::accumulate(threadSums.begin(), threadSums.end(), 0.0);
    return std::exp(-r * T) * (total / numSimulations);
}