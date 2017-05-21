#include "ecs_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <iostream>

int main()
{
	world w;
	position pos;
	health hea;
	attack att;


	for(int i = 0; i < 32; ++i)
	{
		uint64_t e = w.create();
		unsigned p = pos.create(e);
		unsigned h = hea.create(e);
		hea.health_points[h] = 1;
		unsigned a = att.create(e);
		att.range[a] = i;
	}

	

	return 0;
}
