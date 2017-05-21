#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "glm/glm.hpp"

class particle {
private:
    glm::vec2 m_velocity;
    glm::vec2 m_position;
    float m_lifetime;
    bool m_alive;

public:
    particle();
    particle(glm::vec2 & v, const glm::vec2 & p, float l);

    const glm::vec2 & get_velocity();
    const glm::vec2 & get_position();
    float get_lifetime();
    bool is_alive();

    void set_velocity(const glm::vec2 & v);
    void set_position(const glm::vec2 & p);
    void set_lifetime(float l);

    void tick(double dt);
};

#endif // PARTICLE_HPP
