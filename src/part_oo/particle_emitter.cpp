#include "particle_emitter.hpp"

particle_emitter::particle_emitter(const glm::vec2 & position, float emission_rate)
{
    m_position = position;
    m_emission_rate = emission_rate;
}

glm::vec2 particle_emitter::get_position()
{
    return m_position;
}

void particle_emitter::set_position(const glm::vec2 & position)
{
    m_position = position;
}

float particle_emitter::get_emission_rate()
{
    return m_emission_rate;
}

void particle_emitter::set_emission_rate(float rate)
{
    m_emission_rate = rate;
}

int particle_emitter::get_particle_count()
{
    return m_particles.size();
}

void particle_emitter::emit()
{
    m_particles.emplace_back(m_position);
}

void particle_emitter::tick(double dt)
{
    for(int i = 0; i < m_emission_rate*dt; ++i)
        emit();

    for(auto it = m_particles.begin(); it != m_particles.end(); )
    {
        it->tick(dt);
        if(!it->is_alive())
            it = m_particles.erase(it);
        else
            ++it;
    }
}
