#include "ood/consumer_component.hpp"

consumer_component::consumer_component(int type)
{
    m_type = type;
}

int consumer_component::get_type()
{
    return m_type;
}
