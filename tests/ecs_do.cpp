#include "ecs_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <iostream>

float appetites [] = {  20,  10,  10,  10,  10,  10 };
float preys     [] = {  10,  20,  10,  10,  10,  10 };
float libidos   [] = {   1,   1, 0.2,   1,   1,   1 };
float speeds    [] = {  20,  15,  20,  20,  15,  25 };

world w;


int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		printf("usage: %s entity-count increment-period\n", argv[0]);
		return -1;
	}

	size_t entity_count = atoll(argv[1]);
	size_t increment_period = atoll(argv[2]);

	double times[entity_count+1];

	// size_t printed_size = 0;
	time_point tp;
	double time;
	for(size_t i = 0; i < entity_count * increment_period; ++i)
	{
		if(i % increment_period == 0)
		{
			uint64_t e = w.create();
			int s = i%6;
			w.spe.create(e, s);
			w.hea.create(e, appetites[s], (s+1)%6);
			w.mov.create(e, speeds[s]);
			w.pos.create(e, sin(i)*10, cos(i)*10);
			w.rep.create(e, libidos[s]);
			times[w.count()] = 0.0;
		}
		tp = now();
		w.update(1);
		time = elapsed(tp, now());
		times[w.count()] += time;

		// if(printed_size != w.count()) {
		// 	fprintf(stderr, "\r%-10llu %-10f", w.count(), time);
		// 	printed_size = w.count();
		// }
	}
	// fprintf(stderr, "\n");

	for(int i = 0; i <= entity_count; ++i)
		std::cout << i << '\t' << times[i] / (double)increment_period << '\n';

	return 0;
}
