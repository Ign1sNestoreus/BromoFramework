#pragma once

#include <chrono>

class Timer {
public:
    Timer();

    void Reset();
    double Elapsed() const;  // секунды с последнего Reset()
    double Mark();           // секунды с последнего Mark()/Reset() и сброс

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

    TimePoint m_start;
};
