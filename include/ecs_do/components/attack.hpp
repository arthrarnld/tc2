#ifndef ATTACK_HPP
#define ATTACK_HPP

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

	// Properties:
	float * range;
	float * damage;
	float * reload;
	state_type * state;
	float * reload_rem;
};

#endif // ATTACK_HPP
