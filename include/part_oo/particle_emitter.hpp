#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "particle.hpp"
#include <vector>

class particle_emitter {
private:
    std::vector<particle> m_particles;
    float m_emission_rate;

public:
    particle_emitter();
    particle_emitter(float emission_rate);

    void set_emission_rate(float rate);
    float get_emission_rate();

    const particle * get_particle_at(int pos);
    int get_particle_count();

    void emit(const particle & p);

    void tick(double dt);
};

#endif // PARTICLE_EMITTER_HPP
