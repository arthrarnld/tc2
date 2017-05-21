#include "entity.hpp"

#include "common/log.hpp"

entity::entity(id i)
	: m_id(i)
	, m_enabled(true)
{  }

bool entity::is_valid()
{
	return m_id != NIL;
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

// void entity::push_message(base_message * msg)
// {
// 	m_messages.emplace_back(msg);
// }

// void entity::flush_messages(world * w)
// {
// 	for(auto m : m_messages)
// 		for(auto c : m_components)
// 			if(c->handle_message(m.get(), w))
// 				break;
//
// 	m_messages.clear();
// }
