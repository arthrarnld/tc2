#ifndef TEAM_COMPONENT_HPP
#define TEAM_COMPONENT_HPP

#include "base_component.hpp"

class team_component : public base_component
{
public:
	team_component(uint64_t owner, int team);

	void set_team(int team);
	int get_team();

private:
	int m_team;
};

#endif // TEAM_COMPONENT_HPP
