#include "entity.hpp"

uint64_t version(uint64_t e)
{
	return e & ((1 << VERSION_BITS) - 1);
}

uint64_t index(uint64_t e)
{
	return e >> VERSION_BITS;
}
