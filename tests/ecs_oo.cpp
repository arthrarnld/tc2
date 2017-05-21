#include "ecs_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>
#include <stdlib.h>

float ranges[]	= {  20,  10,  10,  10,  10,  10 };
float damages[]	= {  10,  20,  10,  10,  10,  10 };
float reloads[]	= {   1,   1, 0.2,   1,   1,   1 };
float healths[]	= { 100, 100, 100, 200, 100, 100 };
float regens[]	= {   1,   1,   1,   1,   5,   1 };
float speeds[]	= {   1,   1,   1,   1,   1,   2 };

int main()
{
	srandom(time(NULL));

	world w;
	w.push_system(new health_system);
	w.push_system(new attack_system);
	w.push_system(new movement_system);

	for(int i = 0; i < 1000; ++i)
	{
		auto & e1 = w.create();
		int t = i%6;
		e1.add_component<health_component>(healths[t], regens[t]);
		e1.add_component<team_component>(t);
		e1.add_component<attack_component>(ranges[t], damages[t], reloads[t]);
		e1.add_component<position_component>((i % 100)*100, (i / 100)*100);
		e1.add_component<movement_component>(speeds[t]);
	}

	int team;
	bool multiple_teams = true;
	time_point tp;
	double t;
	while(multiple_teams)
	{
		tp = now();
		if(!w.update(1)) break;
		t = elapsed(tp, now());
		log("%llu %f", w.get_entity_count(), t);

		team = -1;
		multiple_teams = false;
		w.for_each([&team, &multiple_teams](entity & e) {
			if(multiple_teams) return;
			auto tc = e.get_component<team_component>();
			if(!tc) return;
			if(team == -1)
				team = tc->team;
			else if(team != tc->team)
				multiple_teams = true;
		});
	} ;

	log("team %d wins!", team);
	return 0;
}
