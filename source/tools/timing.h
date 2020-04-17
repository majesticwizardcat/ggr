#pragma once

#include <chrono>

class Timer {
private:
	std::chrono::duration<float> m_duration;
	std::chrono::steady_clock::time_point m_start;
	bool m_running;

public:
	Timer();
	Timer(const Timer& other);

	void start();
	void stop();
	void addTimer(const Timer& t);
	std::chrono::duration<float> getDuration() const;
};

