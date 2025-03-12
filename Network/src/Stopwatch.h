#pragma once
#include <chrono>
class Stopwatch
{
public:
	void start();
	void stop();
	double getDurationInMS();
private:
	std::chrono::nanoseconds m_timeStarted;
	std::chrono::nanoseconds m_timeStopped;
	std::chrono::nanoseconds m_duration;
};