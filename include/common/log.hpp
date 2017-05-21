#ifndef LOG_HPP
#define LOG_HPP

#define DEBUG 0

void log(const char * fmt, ...);

#if DEBUG
	void debug(const char * fmt, ...);
#else
	#define debug(...) ((void)0)
#endif
void warn(const char * fmt, ...);
void error(const char * fmt, ...);
void fatal(const char * fmt, ...);

#endif // LOG_HPP
