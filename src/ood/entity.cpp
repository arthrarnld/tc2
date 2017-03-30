#include "ood/entity.hpp"

#include "common/log.hpp"

entity::entity(id i)
	: m_id(i)
	, m_enabled(true)
{
	
}

entity::~entity()
{
	
}

uint64_t entity::get_id()
{
	return m_id;
}

uint64_t entity::get_index()
{
	return m_id >> VERSION_BITS;
}

uint64_t entity::get_version()
{
	return m_id & ((1 << VERSION_BITS) - 1);
}

bool entity::is_enabled()
{
	return m_enabled;
}

void entity::disable()
{
	m_enabled = false;
}

void entity::enable()
{
	m_enabled = true;
}

void entity::handle_message(message * msg)
{
	for(auto & c : m_components)
		if(c->handle_message(msg))
			break;
}
