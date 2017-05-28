#include "area_emitter.hpp"
#include "common/util.hpp"

area_emitter::area_emitter(const glm::vec2 & position, float emission_rate, float max_distance)
    : particle_emitter(position, emission_rate)
{
    m_max_distance = max_distance;
}

void area_emitter::emit()
{
    float x = (norm_rand()*2.0f - 1.0f) * m_max_distance;

    m_particles.emplace_back(get_position() + glm::vec2(x, 0.0), glm::vec2(0.0, 1.0));
}

float area_emitter::get_max_distance()
{
    return m_max_distance;
}

void area_emitter::set_max_distance(float max_distance)
{
    m_max_distance = max_distance;
}
