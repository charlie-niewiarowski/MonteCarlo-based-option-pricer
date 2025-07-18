//
// Created by cniew on 7/15/25.
//

#include "Timer.h"

#include <iostream>

Timer::Timer()
{
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    stop();
}


void Timer::stop()
{
    auto end_time = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    std::cout << duration << "us\n" << ms << "ms\n";
}