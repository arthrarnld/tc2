#include "components/health_component.hpp"
#include "common/log.hpp"

#include "world.hpp"
#include "messages/health_message.hpp"
#include "messages/death_message.hpp"

const base_message::id_type health_message::id = base_message::id_gen::get<health_message>();
const base_message::id_type death_message::id = base_message::id_gen::get<death_message>();

bool health_component::handle_message(base_message * m, world * w)
{
	if(m->get_id() != health_message::id)
		return false;

	float val = reinterpret_cast<health_message*>(m)->get_value();
	debug("[health component] handling health message: %f", val);
	m_health = std::max(0.0f, std::min(m_max_health, m_health+val));
	if(m_health == 0)
		w->push_message(new death_message(m_owner));
}
