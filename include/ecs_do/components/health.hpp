#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <cstdint>
#include <unordered_map>


#include "../soa_utils.hpp"

struct health
{
	health(size_t cap = 8)
		: helper(this, cap, &health_points, &max_health, &regen_rate)
	{  }

	SOA_COMPONENT_BASE(health)

	inline size_t create(uint64_t e, float max, float regen) {
		size_t i = create(e);
		health_points[i] = max;
		max_health[i] = max;
		regen_rate[i] = regen;
		return i;
	}


	float * health_points;
	float * max_health;
	float * regen_rate;

};

#endif // HEALTH_COMPONENT_HPP
