#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>

static const uint64_t VERSION_BITS = 8;

uint64_t version(uint64_t e);
uint64_t index(uint64_t e);

#endif // ENTITY_HPP
