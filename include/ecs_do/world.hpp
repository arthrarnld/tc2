#ifndef WORLD_HPP
#define WORLD_HPP

#include <stdint.h>

struct world
{
	uint64_t create();
	void destroy(uint64_t eid);
};

#endif // WORLD_HPP
