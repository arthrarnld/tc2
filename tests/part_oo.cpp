#include "part_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>
#include <iostream>
#include <map>
#include <vector>

void run_measure_tick(size_t iterations, size_t period, size_t emitter_count, size_t passes)
{
    std::map<int, double> times;
    std::map<int, int> occurrences;


    time_point start;
    double taken;

    for(size_t p = 0; p < passes; ++p)
    {
        std::vector<particle_emitter> emitters;
        emitters.reserve(emitter_count);

        for(size_t i = 0; i < emitter_count; ++i)
            emitters.emplace_back(glm::vec2(100.0 * i, 100.0 * i), 1);

        int particle_count;
        for(size_t i = 0; i < iterations; ++i)
        {
            particle_count = 0;

            start = now();
            for(size_t j = 0; j < emitter_count; ++j)
            {
                particle_emitter & e = emitters[j];
                e.tick(1);

                particle_count += e.get_particle_count();
            }
            taken = elapsed(start, now());

            times[particle_count] += taken;
            ++occurrences[particle_count];

            // fprintf(stderr, "\ri: %d\tdt: %-20f", i, taken);

            // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
            if(i % period == 0)
                for(size_t j = 0; j < emitter_count; ++j)
                    emitters[j].set_emission_rate(emitters[j].get_emission_rate() + 1);
        }
    }

    fprintf(stderr, "\n");

    for(auto & p : times)
    {
        std::cout << p.first << '\t' << p.second / occurrences[p.first] << '\n';
    }
}

void run_measure_insertion(size_t emitter_count, size_t passes)
{
    time_point start;
    double taken;

    std::vector<particle_emitter> emitters;

    for(size_t p = 0; p < passes; ++p)
    {
        start = now();
        for(size_t i = 0; i < emitter_count; ++i)
        {
            emitters.emplace_back(glm::vec2(100.0 * i, 100.0 * i), 1);
        }
        taken += elapsed(start, now());

        debug("\ri: %d\tdt: %-20f", p, taken);
    }

    std::cout << emitter_count << '\t' << (taken / passes) << '\n';
}

int main(int argc, char ** argv)
{
    enum { TICK, INSERTION } test = TICK;

    size_t iterations;
    size_t period;
    size_t emitter_count;
    size_t passes;

	int c;
	while((c = getopt(argc, argv, "ti")) != -1)
		switch(c)
		{
			case 't':
				test = TICK;
				break;
			case 'i':
				test = INSERTION;
				break;
			case '?':
				fprintf(stderr, "unknown option '-%c'\n", optopt);
			default:
				return -1;
		}

	switch(test)
	{
		case TICK:
			if(optind != argc-4)
				fatal("tick test requires four arguments: <iterations> <increment period> <emitter count> <passes>");
            iterations = atoll(argv[optind++]);
            period = atoll(argv[optind++]);
			emitter_count = atoll(argv[optind++]);
            passes = atoll(argv[optind++]);
            run_measure_tick(iterations, period, emitter_count, passes);
			break;
		case INSERTION:
			if(optind != argc-2)
				fatal("insertion test requires one argument: <emitter count> <passes>");
			emitter_count = atoll(argv[optind++]);
            passes = atoll(argv[optind++]);
            run_measure_insertion(emitter_count, passes);
			break;
	}

	return 0;
}
