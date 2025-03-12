#include "Stopwatch.h"

void Stopwatch::start()
{
	m_timeStarted = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop()
{
	m_timeStopped = std::chrono::high_resolution_clock::now();
}

double Stopwatch::getDurationInMS()
{
	return std::chrono::duration<double, std::milli>(m_timeStopped - m_timeStarted).count();
}

double Stopwatch::getDurationInSeconds()
{
	return std::chrono::duration<double>(m_timeStopped - m_timeStarted).count();
}
