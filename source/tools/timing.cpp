#include "tools/timing.h"

Timer::Timer() : m_running(false) { }
Timer::Timer(const Timer& other) : m_duration(other.m_duration),
	m_start(other.m_start), m_running(other.m_running) { }

void Timer::start() {
	if (!m_running) {
		m_start = std::chrono::steady_clock::now();
		m_running = true;
	}
}

void Timer::stop() {
	if (m_running) {
		m_duration = std::chrono::steady_clock::now() - m_start;
		m_running = false;
	}
}

void Timer::addTimer(const Timer& t) {
	m_duration += t.getDuration();
}

std::chrono::duration<float> Timer::getDuration() const {
	return m_duration;
}

