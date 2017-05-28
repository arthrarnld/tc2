#include "particle.hpp"

particle::particle(const glm::vec2 & p, const glm::vec2 & v, float l)
{
    m_position = p;
    m_velocity = v;
    m_lifetime = l;
    m_alive = true;
}

const glm::vec2 & particle::get_velocity()
{
    return m_velocity;
}

const glm::vec2 & particle::get_position()
{
    return m_position;
}

float particle::get_lifetime()
{
    return m_lifetime;
}

bool particle::is_alive() {
    return m_alive;
}

void particle::set_velocity(const glm::vec2 & v)
{
    m_velocity = v;
}

void particle::set_position(const glm::vec2 & p)
{
    m_position = p;
}

void particle::set_lifetime(float l)
{
    m_lifetime = l;
}

void particle::tick(double dt)
{
    m_lifetime -= dt;
    if(m_lifetime > 0)
        m_position += m_velocity;
    else
        m_alive = false;
}
