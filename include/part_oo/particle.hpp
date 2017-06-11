#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "glm/glm.hpp"

class particle {
private:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_lifetime;

public:
    particle(const glm::vec2 & position);
    particle(const glm::vec2 & p, const glm::vec2 & v, float l);

    const glm::vec2 & get_position();
    const glm::vec2 & get_velocity();
    float get_lifetime();
    bool is_alive();

    void set_position(const glm::vec2 & p);
    void set_velocity(const glm::vec2 & v);
    void set_lifetime(float l);

    void tick(double dt);
};

#endif // PARTICLE_HPP
