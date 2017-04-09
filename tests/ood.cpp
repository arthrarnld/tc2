#include "02_ood_ecs/world.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>

int main()
{
	auto t1 = now();
	sleep(2);
	auto t2 = now();
	log("time: %lf", elapsed(t1, t2));
	warn("dkjsdllkdsj");
	error("lkdlkdjsldj");
	return 0;
}
