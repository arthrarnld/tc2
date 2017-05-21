#ifndef ATTACK_SYSTEM_HPP
#define ATTACK_SYSTEM_HPP

#include "../components/attack.hpp"
#include "../components/position.hpp"
#include "../components/team.hpp"
#include "../components/movement.hpp"
#include "../components/health.hpp"

struct world;

bool update_attack(world * w, double dt);

#endif // ATTACK_SYSTEM_HPP
