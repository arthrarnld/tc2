#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "glm/glm.hpp"
#include "common/array.hpp"

struct particle_system
{
    array<glm::vec2> positions;
    array<glm::vec2> velocities;
    array<double> lifetimes;

    void tick(double dt);
    void emit();
};



#endif // PARTICLE_SYSTEM_HPP
