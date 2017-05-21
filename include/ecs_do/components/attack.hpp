#ifndef ATTACK_COMPONENT_HPP
#define ATTACK_COMPONENT_HPP

#include "../soa_utils.hpp"


struct attack
{
	attack(size_t c = 8)
		: helper(this,
			c,
			&range,
			&damage,
			&reload,
			&state,
			&reload_rem )
	{  }

	enum state_type { IDLE, RELOADING, ENGAGED };

	SOA_COMPONENT_BASE(attack)

	inline size_t create(uint64_t e, float rg, float dmg, float rld) {
		size_t idx = create(e);
		range[idx] = rg;
		damage[idx] = dmg;
		reload[idx] = rld;
		state[idx] = IDLE;
		return idx;
	}

	// Properties:
	float * range;
	float * damage;
	float * reload;
	state_type * state;
	float * reload_rem;
};

#endif // ATTACK_COMPONENT_HPP
