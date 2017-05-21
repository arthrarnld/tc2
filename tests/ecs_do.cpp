#include "ecs_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <iostream>

float ranges[]	= {  20,  10,  10,  10,  10,  10 };
float damages[]	= {  10,  20,  10,  10,  10,  10 };
float reloads[]	= {   1,   1, 0.2,   1,   1,   1 };
float healths[]	= { 100, 100, 100, 200, 100, 100 };
float regens[]	= {   1,   1,   1,   1,   5,   1 };
float speeds[]	= {   1,   1,   1,   1,   1,   2 };

world w;


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

	for(int i = 0; i < entity_count; ++i)
	{
		auto e = w.create();
		int t = i%6;
		w.hea.create(e, healths[t], regens[t]);
		w.tea.create(e, t);
		w.att.create(e, ranges[t], damages[t], reloads[t]);
		w.pos.create(e, (i % 100)*100, (i/100)*100);
		w.mov.create(e, speeds[t]);
		times[i] = 0.0;
		occurrences[i] = 0;
	}

	int t = -1;
	size_t printed_size = 0;
	bool multiple_teams = true;
	time_point tp;
	double time;
	while(multiple_teams)
	{
		tp = now();
		w.update(1);
		time = elapsed(tp, now());
		times[w.count()] += time;
		++occurrences[w.count()];

		if(printed_size != w.count()) {
			fprintf(stderr, "\r%-10llu %-10f", w.count(), time);
			printed_size = w.count();
		}

		t = -1;
		multiple_teams = false;
		for(size_t i = 0; i < w.tea.size() && !multiple_teams; ++i)
			if(t == -1)
				t = w.tea.team_id[i];
			else if(t != w.tea.team_id[i])
				multiple_teams = true;
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
