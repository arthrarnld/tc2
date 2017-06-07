#include "common/log.hpp"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#define C_RED(X)     "\x1b[31;1m" X "\x1b[0m"
#define C_GREEN(X)   "\x1b[32;1m" X "\x1b[0m"
#define C_YELLOW(X)  "\x1b[33;1m" X "\x1b[0m"
#define C_BLUE(X)    "\x1b[34;1m" X "\x1b[0m"
#define C_CYAN(X)    "\x1b[36;1m" X "\x1b[0m"
#define C_MAGENTA(X) "\x1b[35;1m" X "\x1b[0m"

size_t log_iteration = 0;

void log(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, C_GREEN("  LOG "));
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);
}

#ifdef DEBUG
void debug(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, C_BLUE("DEBUG ") "%-5zu ", log_iteration);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);
}
#endif

void warn(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, C_YELLOW(" WARN "));
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);
}

void error(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, C_RED("ERROR "));
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);
}

void fatal(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, C_RED("FATAL "));
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");

	va_end(args);
	std::exit(-1);
}
