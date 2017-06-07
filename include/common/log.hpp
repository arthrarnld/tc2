#ifndef LOG_HPP
#define LOG_HPP

#include <cstddef>

extern size_t log_iteration;

void log(const char * fmt, ...);

#ifdef DEBUG
	void debug(const char * fmt, ...);
#else
	#define debug(...) ((void)0)
#endif
void warn(const char * fmt, ...);
void error(const char * fmt, ...);
void fatal(const char * fmt, ...);

#endif // LOG_HPP
