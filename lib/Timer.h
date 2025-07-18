//
// Created by cniew on 7/15/25.
//

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    ~Timer();
    void stop();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};



#endif //TIMER_H
