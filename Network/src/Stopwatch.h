#pragma once
#include <chrono>
class Stopwatch
{
public:
	void start();
	void stop();
	double getDurationInMS();
	double getDurationInSeconds();
	std::chrono::time_point<std::chrono::steady_clock> m_timeStarted;
	std::chrono::time_point<std::chrono::steady_clock> m_timeStopped;
};