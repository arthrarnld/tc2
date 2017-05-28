#include "cone_emitter.hpp"

#include "common/util.hpp"
#include "glm/gtx/rotate_vector.hpp"

const glm::vec2 cone_emitter::UP = glm::vec2(0.0, 1.0);

cone_emitter::cone_emitter(float angle_rad)
{
    m_angle = angle_rad;
}

void cone_emitter::emit()
{
    float rotation = norm_rand() * m_angle - m_angle;

    m_particles.emplace_back(get_position(), glm::rotate(UP, rotation));
}

float cone_emitter::get_angle()
{
    return m_angle;
}

void cone_emitter::set_angle(float angle_rad)
{
    m_angle = angle_rad;
}
