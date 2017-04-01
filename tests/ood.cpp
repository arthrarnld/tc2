#include "ood/world.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>

int main()
{
	auto t1 = now();
	auto t2 = now();
	log("time: %lf\n", elapsed(t1, t2));
	return 0;
}
