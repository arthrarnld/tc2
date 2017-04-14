#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <algorithm>

#include "base_component.hpp"
#include "../entity.hpp"

class health_component : public base_component
{
public:
	health_component(uint64_t owner, float max, float regen)
		: base_component(owner)
		, m_health(max)
		, m_max_health(max)
		, m_regen_rate(regen)
		{  }

	float get_health()
		{ return m_health; }
	void set_health(float h)
		{ m_health = std::min(h, m_max_health); }

	float get_max_health()
		{ return m_max_health; }
	void set_max_health(float m)
		{ m_max_health = m; m_health = std::min(m, m_health); }

	float get_regen_rate()
		{ return m_regen_rate; }
	void set_regen_rate(float r)
		{ m_regen_rate = r; }

	bool handle_message(base_message * m, world * w) override;
private:
	float m_health;
	float m_max_health;
	float m_regen_rate;
};

#endif // HEALTH_COMPONENT_HPP
