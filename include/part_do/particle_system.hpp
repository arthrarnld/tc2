#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "glm/glm.hpp"
#include "common/array.hpp"

struct particle_system
{
    //// EMITTERS //////////////////////////////////////////////////////////////
    enum mode_type { LINE, CONE, AREA };

    array<float> e_emission_rates;
    array<glm::vec2> e_positions;

    // cone
    array<float> cone_angles;

    // area
    array<float> area_max_distances;

    //// PARTICLES /////////////////////////////////////////////////////////////
    array<int> p_from;
    array<glm::vec2> p_positions;
    array<glm::vec2> p_velocities;
    array<double> p_lifetimes;

    //// SYSTEM ////////////////////////////////////////////////////////////////
    size_t partitions[2] = { 0, 0 };

    static const glm::vec2 UP;

    particle_system();

    void new_line_emitter(const glm::vec2 & position, float emission_rate);
    void new_cone_emitter(const glm::vec2 & position, float emission_rate, float angle_rad);
    void new_area_emitter(const glm::vec2 & position, float emission_rate, float max_distance);

    int count();

    void tick(double dt);
    void emit(int from, const glm::vec2 & p, const glm::vec2 & v, double l, double dt);

};

#endif // PARTICLE_SYSTEM_HPP
