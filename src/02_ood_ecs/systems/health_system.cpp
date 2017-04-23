#include "systems/health_system.hpp"

#include "common/log.hpp"

#include "messages/health_message.hpp"
#include "messages/death_message.hpp"

bool health_system::update(world & w, double dt)
{
	for(auto & pair : w)
	{
		auto e = pair.second;
		if(e->has_component<health_component>())
		{
			auto & hc = *(e->get_component<health_component>());
			if(hc.health <= 0)
				w.push_message(new death_message(e->get_id()));
			else
				hc.health = std::min((double)hc.max_health, hc.health+hc.regen_rate*dt);
		}
	}
	return true;
}

bool health_system::handle_message(base_message * msg, world & w)
{
	if(msg->get_id() == health_message::id)
	{
		auto m = static_cast<health_message*>(msg);
		auto from = m->sender;
		auto to = m->recipient;
		auto e = w.get(m->recipient);
		if(!e)
			return true;
		auto hc = e->get_component<health_component>();
		hc->health -= m->value;
		debug("%llu takes %.2f damage [HP = %.2f]", to, m->value, hc->health);
		return true;
	}
	return false;
}
