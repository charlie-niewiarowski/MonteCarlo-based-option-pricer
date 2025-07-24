//
// Created by cniew on 7/16/25.
//

#include "montecarlo.h"

// Computations
#include <immintrin.h>
#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <sleef.h>

// Benchmarking
#include "../lib/Timer.h"

static const __m256d zero_pd = _mm256_set1_pd(0.0);
static const __m256d one_pd = _mm256_set1_pd(1.0);

MonteCarlo::MonteCarlo(double _S0, double _K, double _r, double _sigma, double _T, int _numSimulations)
    : S0(_S0), K(_K), r(_r), sigma(_sigma), T(_T), numSimulations(_numSimulations), rng(std::random_device{}())
{}


__m256d MonteCarlo::generateGaussianNoise(__m256d mean, __m256d stddev) {
    __m256d u1 = rng.dnext4(0.0, 1.0);
    __m256d u2 = rng.dnext4(0.0, 1.0);

    __m256d coefficient = Sleef_sqrtd4_avx2(_mm256_mul_pd(_mm256_set1_pd(-2.0), Sleef_logd4_u10avx2(u1)));
    __m256d theta = _mm256_mul_pd(_mm256_set1_pd(2.0 * M_PI), u2);
    return _mm256_mul_pd(coefficient, Sleef_cosd4_u10avx2(theta));
}

__m256d MonteCarlo::singleThreadCall(__m256d left, __m256d rightExponent, int pathsPerThread) {
    __m256d payoffSum = _mm256_setzero_pd();
    for (int i = 0; i < pathsPerThread; i += 4) {
        __m256d Z = generateGaussianNoise(zero_pd, one_pd);
        __m256d ST = _mm256_mul_pd(left, Sleef_expd4_u10avx2(_mm256_mul_pd(rightExponent, Z)));

        __m256d terminal = _mm256_max_pd(_mm256_sub_pd(ST, _mm256_set1_pd(K)), zero_pd);
        payoffSum = _mm256_add_pd(payoffSum, terminal);
    }

    return payoffSum;
}

__m256d MonteCarlo::singleThreadPut(__m256d left, __m256d rightExponent, int pathsPerThread) {
    __m256d payoffSum = _mm256_setzero_pd();
    for (int i = 0; i < pathsPerThread; i += 4) {
        __m256d Z = generateGaussianNoise(zero_pd, one_pd);
        __m256d ST = _mm256_mul_pd(left, Sleef_expd4_u10avx2(_mm256_mul_pd(rightExponent, Z)));

        __m256d terminal = _mm256_max_pd(_mm256_sub_pd(_mm256_set1_pd(K), ST), zero_pd);
        payoffSum = _mm256_add_pd(payoffSum, terminal);
    }

    return payoffSum;
}

double MonteCarlo::priceCallOption() {
    int threads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> workers;
    std::vector<double> partial_sums(threads, 0.0);  // avoid shared write
    int pathsPerThread = numSimulations / threads;

    __m256d left = _mm256_set1_pd(S0 * std::exp((r - 0.5 * sigma * sigma) * T));
    __m256d rightExponent = _mm256_set1_pd(sigma * std::sqrt(T));

    std::vector<double> threadSums(threads, 0.0);
    for (unsigned int i = 0; i < threads; ++i) {
        workers.emplace_back([&, i, pathsPerThread]() {
            __m256d vectorizedTotal = singleThreadCall(left, rightExponent, pathsPerThread);

            double vec[4];
            _mm256_storeu_pd(vec, vectorizedTotal);
            threadSums[i] = vec[0] + vec[1] + vec[2] + vec[3];
        });
    }

    for (auto& t : workers) t.join();

    double total = std::accumulate(threadSums.begin(), threadSums.end(), 0.0);
    double res = std::exp(-r * T) * (total / numSimulations);

    totalCallPrice += res; callIterations += 1;
    return res;
}

double MonteCarlo::pricePutOption() {
    int threads = static_cast<int>(std::thread::hardware_concurrency());
    std::vector<std::thread> workers;
    std::vector<double> partial_sums(threads, 0.0);  // avoid shared write
    int pathsPerThread = numSimulations / threads;

    __m256d left = _mm256_set1_pd(S0 * std::exp((r - 0.5 * sigma * sigma) * T));
    __m256d rightExponent = _mm256_set1_pd(sigma * std::sqrt(T));

    std::vector<double> threadSums(threads, 0.0);
    for (unsigned int i = 0; i < threads; ++i) {
        workers.emplace_back([&, i, pathsPerThread]() {
            __m256d vectorizedTotal = singleThreadPut(left, rightExponent, pathsPerThread);

            double vec[4];
            _mm256_storeu_pd(vec, vectorizedTotal);
            threadSums[i] = vec[0] + vec[1] + vec[2] + vec[3];
        });
    }

    for (auto& t : workers) t.join();

    double total = std::accumulate(threadSums.begin(), threadSums.end(), 0.0);
    double res = std::exp(-r * T) * (total / numSimulations);

    totalPutPrice += res; putIterations += 1;
    return res;
}

///////////////////////////////// Benchmarking ////////////////////////////////

double MonteCarlo::avgCallPrice(const int N) {
    if (callIterations < N) {
        for (int i = callIterations; i < N; ++i) {
            priceCallOption();
        }
    }

    return totalCallPrice / callIterations;
}

double MonteCarlo::avgPutPrice(const int N) {
    if (putIterations < N) {
        for (int i = putIterations; i < N; ++i) {
            pricePutOption();
        }
    }

    return totalPutPrice / putIterations;
}

double MonteCarlo::stdErrorCall(const int N) {
    double diff = 0;
    double startingCallAvg = avgCallPrice(N);
    for (unsigned int i = 0; i < N; i++) {
        diff += std::pow(startingCallAvg - priceCallOption(), 2);
    }

    const double stdDeviation = std::sqrt(diff / (N - 1));
    return stdDeviation / std::sqrt(N);
}

double MonteCarlo::stdErrorPut(const int N) {
    double diff = 0;
    double startingPutAvg = avgPutPrice(N);
    for (unsigned int i = 0; i < N; i++) {
        diff += std::pow(startingPutAvg - pricePutOption(), 2);
    }

    const double stdDeviation = std::sqrt(diff / (N - 1));
    return stdDeviation / std::sqrt(N);
}

double MonteCarlo::avgCallTime(const int N) {
    double totalTime = 0;
    Timer timer;
    for (int i = 0; i < N; ++i) {
        priceCallOption();
    }
    totalTime += timer.stop();
    return totalTime / N;
}

double MonteCarlo::avgPutTime(const int N) {
    double totalTime = 0;
    Timer timer;
    for (int i = 0; i < N; ++i) {
        pricePutOption();
    }
    totalTime += timer.stop();
    return totalTime / N;
}