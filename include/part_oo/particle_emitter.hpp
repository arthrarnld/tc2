#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "particle.hpp"
#include "glm/glm.hpp"
#include <vector>

class particle_emitter {
private:
    glm::vec2 m_position;
    float m_emission_rate;

protected:
    std::vector<particle> m_particles;
    virtual void emit();

public:
    particle_emitter(const glm::vec2 & position = glm::vec2(), float emission_rate = 1);

    glm::vec2 get_position();
    void set_position(const glm::vec2 & position);

    void set_emission_rate(float rate);
    float get_emission_rate();

    int get_particle_count();

    void tick(double dt);
};

#endif // PARTICLE_EMITTER_HPP
