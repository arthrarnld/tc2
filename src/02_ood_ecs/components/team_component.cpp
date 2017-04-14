#include "components/team_component.hpp"

team_component::team_component(uint64_t owner, int team)
	: base_component(owner)
	, m_team(team)
{  }

void team_component::set_team(int team)
{
	m_team = team;
}

int team_component::get_team()
{
	return m_team;
}
