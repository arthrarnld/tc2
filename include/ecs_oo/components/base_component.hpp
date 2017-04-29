#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include <cstdint>

class world;

class base_component
{
public:
	base_component(uint64_t owner) : owner(owner) {  }

	uint64_t owner;
};

#endif // BASE_COMPONENT_HPP
