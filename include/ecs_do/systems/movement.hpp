#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "../components/movement.hpp"
#include "../components/position.hpp"

struct world;

bool update_movement(world * w, double dt);

#endif // MOVEMENT_SYSTEM_HPP
