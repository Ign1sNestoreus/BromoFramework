#include "timer.hh"

Timer::Timer() {
    Reset();
}

void Timer::Reset() {
    m_start = Clock::now();
}

double Timer::Elapsed() const {
    auto now = Clock::now();
    auto duration = std::chrono::duration<double>(now - m_start);
    return duration.count();
}

double Timer::Mark() {
    auto now = Clock::now();
    auto duration = std::chrono::duration<double>(now - m_start);
    m_start = now;
    return duration.count();
}
