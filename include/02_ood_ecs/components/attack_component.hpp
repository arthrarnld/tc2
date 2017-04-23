#ifndef ATTACK_COMPONENT_HPP
#define ATTACK_COMPONENT_HPP

#include "base_component.hpp"

class attack_component : public base_component
{
public:
	enum state_type { IDLE, RELOADING, ENGAGED };

	attack_component(uint64_t owner, float range, float damage, float reload)
		: base_component(owner)
		, range(range)
		, damage(damage)
		, reload(reload)
		, state(IDLE)
	{

	}

	float range;
	float damage;
	float reload;
	state_type state;
	float reload_rem;
};

#endif // ATTACK_COMPONENT_HPP
