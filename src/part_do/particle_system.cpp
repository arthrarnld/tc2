#import "particle_system.hpp"

void particle_system::create_emitter(const glm::vec2 & position, float emission_rate)
{
    e_positions.push_back(position);
    e_emission_rates.push_back(emission_rate);
}

size_t particle_system::size()
{
    return e_positions.size();
}

int particle_system::count()
{
    return p_from.size();
}

void particle_system::tick(double dt)
{
    // tick emitters
    for(size_t i = 0; i < size(); ++i)
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
