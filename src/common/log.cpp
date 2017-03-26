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


void log(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf("LOG ");
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
}

void warn(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf(C_YELLOW("WRN "));
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
}

void error(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf(C_RED("ERR "));
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
}

void fatal(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	printf(C_RED("FATAL "));
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
	std::exit(-1);
}
