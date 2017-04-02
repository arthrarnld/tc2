#ifndef CONSUMER_COMPONENT_HPP
#define CONSUMER_COMPONENT_HPP

#include "base_component.hpp"

class consumer_component : public base_component {
public:
    consumer_component(int type);

    int get_type();
private:
    int m_type;
};

////////////////////////////////////////////////////////////////////////////////
// Messages

#include "message.hpp"
#include "id_generator.hpp"

class entity_consumed : public message
{
public:
	entity_consumed(consumer_component * sender, entity::id consumed)
		: message(sender)
        , m_consumed(consumed)
	{  }

	uint64_t get_id()
	{
		return id_generator<message>::get<entity_consumed>();
	}

    entity::id get_consumed()
    {
        return m_consumed;
    }

private:
    entity::id m_consumed;
};



#endif // CONSUMER_COMPONENT_HPP
