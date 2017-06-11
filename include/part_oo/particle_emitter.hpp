#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "particle.hpp"
#include "glm/glm.hpp"
#include <vector>

class particle_emitter {
private:
    std::vector<particle> m_particles;
    glm::vec2 m_position;
    float m_emission_rate;

public:
    particle_emitter();
    particle_emitter(const glm::vec2 & position, float emission_rate);

    void set_emission_rate(float rate);
    float get_emission_rate();

    int get_particle_count();

    void emit(const particle & p);

    void tick(double dt);
};

#endif // PARTICLE_EMITTER_HPP
