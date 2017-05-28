#include "particle_system.hpp"
#include "common/util.hpp"
#include "glm/gtx/rotate_vector.hpp"

const glm::vec2 particle_system::UP = glm::vec2(0.0, 1.0);

particle_system::particle_system() { }

void particle_system::new_line_emitter(const glm::vec2 & position, float emission_rate)
{
    e_positions.push_back(position);
    e_emission_rates.push_back(emission_rate);
    move(e_positions.size()-1, 0, [this](size_t a, size_t b) {
        this->e_positions.swap(a, b);
        this->e_emission_rates.swap(a, b);
    }, partitions);
}

void particle_system::new_cone_emitter(const glm::vec2 & position, float emission_rate, float angle_rad)
{
    e_positions.push_back(position);
    e_emission_rates.push_back(emission_rate);
    cone_angles.push_back(angle_rad);
    move(e_positions.size()-1, 1, [this](size_t a, size_t b) {
        this->e_positions.swap(a, b);
        this->e_emission_rates.swap(a, b);
    }, partitions);
}

void particle_system::new_area_emitter(const glm::vec2 & position, float emission_rate, float max_distance)
{
    e_positions.push_back(position);
    e_emission_rates.push_back(emission_rate);
    area_max_distances.push_back(max_distance);
}

int particle_system::tick(double dt)
{
    // tick emitters
    for(size_t i = 0; i < partitions[0]; ++i)
    {
        emit(i, e_positions[i], glm::vec2(1.0,1.0), 10, dt);
    }
    for(size_t i = partitions[0]; i < partitions[1]; ++i)
    {
        float ang = cone_angles[i - partitions[0]];
        float rotation = norm_rand() * ang - ang;

        emit(i, e_positions[i], glm::rotate(UP, rotation), 10, dt);
    }
    for(size_t i = partitions[1]; i < e_positions.size(); ++i)
    {
        float dist = area_max_distances[i - partitions[1]];
        float x = (norm_rand()*2.0f - 1.0f) * dist;

        emit(i, e_positions[i] + glm::vec2(x, 0.0), glm::vec2(1.0,1.0), 10, dt);
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
