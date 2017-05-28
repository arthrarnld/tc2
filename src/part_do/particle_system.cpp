#import "particle_system.hpp"

particle_system::particle_system()
{
    m_last_active_emitter = -1;
}

void particle_system::create_emitter(const glm::vec2 & position, float emission_rate)
{
    e_positions.push_back(position);
    e_emission_rates.push_back(emission_rate);
    m_last_active_emitter++;
}

void particle_system::disable_emitter(int index)
{
    if(m_last_active_emitter > 0)
    {
        e_positions.swap(index, m_last_active_emitter);
        e_emission_rates.swap(index, m_last_active_emitter);
    }

    m_last_active_emitter--;
}

int particle_system::tick(double dt)
{
    // tick emitters
    for(size_t i = 0; i <= m_last_active_emitter; ++i)
    {
        emit(i, glm::vec2(0.0,0.0), glm::vec2(1.0,1.0), 10, dt);
    }

    // tick particles
    for(size_t i = 0; i < p_from.size(); )
    {
        if(p_lifetimes[i] > 0)
        {
            p_positions[i] += p_velocities[i];
            p_lifetimes[i] -= dt;
            ++i;
        }
        else
        {
            p_from.remove(i);
            p_positions.remove(i);
            p_velocities.remove(i);
            p_lifetimes.remove(i);
        }
    }

    particle_count = p_from.size();
    return particle_count;
}

void particle_system::emit(int from, const glm::vec2 & p, const glm::vec2 & v, double l, double dt)
{
    for(size_t i = 0; i < e_emission_rates[from] * dt; ++i)
    {
        p_from.push_back(from);
        p_positions.push_back(e_positions[from] + p);
        p_velocities.push_back(v);
        p_lifetimes.push_back(l);
    }
}
