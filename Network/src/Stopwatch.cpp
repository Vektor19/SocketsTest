#include "Stopwatch.h"

void Stopwatch::start()
{
	m_timeStarted = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop()
{
	m_timeStopped = std::chrono::high_resolution_clock::now();
}
