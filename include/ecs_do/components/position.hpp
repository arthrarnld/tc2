#ifndef POSITION_MANAGER_HPP
#define POSITION_MANAGER_HPP

#include <stdint.h>
#include <unordered_map>
#include <glm/glm.hpp>

#include "../soa_utils.hpp"

struct position
{
	position(size_t c = 8)
		: helper(this, c, &pos)
	{  }

	SOA_COMPONENT_BASE(position)

	glm::vec3 * pos;
};


#endif // POSITION_MANAGER_HPP
