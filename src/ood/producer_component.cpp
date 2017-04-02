#include "ood/producer_component.hpp"

producer_component::producer_component(int type)
{
    m_type = type;
}

producer_component::get_type()
{
    return m_type;
}
