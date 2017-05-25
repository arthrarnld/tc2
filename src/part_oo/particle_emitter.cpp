#include "particle_emitter.hpp"

particle_emitter::particle_emitter()
{
    m_position = glm::vec2(0.0, 0.0);
    m_emission_rate = 1;
}

particle_emitter::particle_emitter(const glm::vec2 & position, float emission_rate)
{
    m_position = position;
    m_emission_rate = emission_rate;
}

float particle_emitter::get_emission_rate()
{
    return m_emission_rate;
}

void particle_emitter::set_emission_rate(float rate)
{
    m_emission_rate = rate;
}

const particle * particle_emitter::get_particle_at(int pos)
{
    if(pos > 0 && pos < m_particles.size())
        return &m_particles[pos];
    return NULL;
}

int particle_emitter::get_particle_count()
{
    return m_particles.size();
}

void particle_emitter::emit(const particle & p)
{
    m_particles.push_back(p);
}

void particle_emitter::tick(double dt)
{
    for(int i = 0; i < m_emission_rate*dt; ++i)
        emit(particle(m_position));

    for (auto & particle : m_particles)
        particle.tick(dt);

    auto it = m_particles.begin();
    while(it != m_particles.end())
    {
        if(!it->is_alive())
            it = m_particles.erase(it);
        else
            ++it;
    }
}
