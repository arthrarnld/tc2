#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>

static const uint64_t VERSION_BITS = 8;

inline uint64_t version(uint64_t e)
{
	return e & ((1 << VERSION_BITS) - 1);
}

inline uint64_t index(uint64_t e)
{
	return e >> VERSION_BITS;
}

#endif // ENTITY_HPP
