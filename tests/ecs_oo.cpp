#include "ecs_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

// #include <unistd.h>
// #include <stdlib.h>
#include <iostream>

float ranges[]	= {  20,  10,  10,  10,  10,  10 };
float damages[]	= {  10,  20,  10,  10,  10,  10 };
float reloads[]	= {   1,   1, 0.2,   1,   1,   1 };
float healths[]	= { 100, 100, 100, 200, 100, 100 };
float regens[]	= {   1,   1,   1,   1,   5,   1 };
float speeds[]	= {   1,   1,   1,   1,   1,   2 };



int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("usage: %s entity-count\n", argv[0]);
		return -1;
	}

	size_t entity_count = atoll(argv[1]);

	double times[entity_count+1];
	size_t occurrences[entity_count+1];

	world w;
	w.push_system(new health_system);
	w.push_system(new attack_system);
	w.push_system(new movement_system);

	for(int i = 0; i < entity_count; ++i)
	{
		auto & e1 = w.create();
		int t = i%6;
		e1.add_component<health_component>(healths[t], regens[t]);
		e1.add_component<team_component>(t);
		e1.add_component<attack_component>(ranges[t], damages[t], reloads[t]);
		e1.add_component<position_component>((i % 100)*100, (i / 100)*100);
		e1.add_component<movement_component>(speeds[t]);
		times[i] = 0.0;
		occurrences[i] = 0;
	}

	int team;
	bool multiple_teams = true;
	time_point tp;
	double time;
	size_t printed_size = 0;
	while(multiple_teams)
	{
		tp = now();
		if(!w.update(1)) break;
		time = elapsed(tp, now());
		times[w.get_entity_count()] += time;
		++occurrences[w.get_entity_count()];

		if(printed_size != w.get_entity_count()) {
			fprintf(stderr, "\r%-10llu %-10f", w.get_entity_count(), time);
			printed_size = w.get_entity_count();
		}

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
	}
	fprintf(stderr, "\n");

	for(int i = 0; i <= entity_count; ++i)
	{
		if(occurrences[i] == 0)
			continue;
		std::cout << i << '\t' << times[i] / (double)occurrences[i] << '\n';
	}

	return 0;
}
