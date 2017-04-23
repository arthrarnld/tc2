#ifndef ATTACK_SYSTEM_HPP
#define ATTACK_SYSTEM_HPP

#include "base_system.hpp"

class attack_system : public base_system
{
public:
	attack_system() = default;

	bool update(world & w, double dt) override;
};

#endif // ATTACK_SYSTEM_HPP
