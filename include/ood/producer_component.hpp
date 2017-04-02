#ifndef PRODUCER_COMPONENT_HPP
#define PRODUCER_COMPONENT_HPP

#include "base_component.hpp"

class producer_component : public base_component {
public:
    producer_component(int type);

private:
    int m_type;
};

#endif // PRODUCER_COMPONENT_HPP
