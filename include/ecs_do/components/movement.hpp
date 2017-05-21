#ifndef MOVEMENT_COMPONENT_HPP
#define MOVEMENT_COMPONENT_HPP

#include "../soa_utils.hpp"

struct movement
{
	movement(size_t c = 8)
		: helper(this, c,
			&target,
			&speed )
	{  }

	SOA_COMPONENT_BASE(movement)

	inline size_t create(uint64_t e, float speed) {
		size_t i = create(e);
		this->speed[i] = speed;
		return i;
	}


	uint64_t * target;
	float * speed;
};

#endif // MOVEMENT_COMPONENT_HPP
