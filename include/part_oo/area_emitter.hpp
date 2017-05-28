#ifndef AREA_EMITTER_HPP
#define AREA_EMITTER_HPP

#include "particle_emitter.hpp"

class area_emitter : public particle_emitter {
private:
    float m_max_distance;

protected:
    void emit() override;

public:
    area_emitter(float max_distance = 10);

    float get_max_distance();
    void set_max_distance(float max_distance);
};

#endif // AREA_EMITTER_HPP
