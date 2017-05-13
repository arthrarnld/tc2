#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <cstdint>
#include <unordered_map>


#include "../soa_utils.hpp"

struct health
{
	health(size_t cap = 8)
		: helper(this, cap, &health_points, &max_regen, &regen_rate)
	{  }

	SOA_COMPONENT_BASE(health)

	float * health_points;
	float * max_regen;
	float * regen_rate;

};

#endif // HEALTH_COMPONENT_HPP
