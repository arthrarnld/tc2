#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

using clock_type = std::chrono::high_resolution_clock;
using time_point = clock_type::time_point;

time_point now() { return clock_type::now(); }

double elapsed(const time_point & start, const time_point & end)
{
	return (end-start).count()/1000000000.0;
}

#endif // TIME_HPP
