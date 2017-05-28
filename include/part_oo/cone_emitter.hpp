#ifndef CONE_EMITTER_HPP
#define CONE_EMITTER_HPP

#include "particle_emitter.hpp"
#include <cstdlib>

class cone_emitter : public particle_emitter {
private:
    static const glm::vec2 UP;
    float m_angle;

protected:
    void emit() override;

public:
    cone_emitter(const glm::vec2 & position, float emission_rate, float angle_rad);

    float get_angle();
    void set_angle(float angle_rad);
};

#endif // CONE_EMITTER_HPP
