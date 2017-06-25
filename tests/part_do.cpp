#include "part_do.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <iostream>
#include <map>
#include <cmath>
#include <unistd.h>

std::map<int, double> times;
std::map<int, int> occurrences;

void run_measure_tick(size_t iterations, size_t period, size_t emitter_count, size_t passes)
{
    time_point start;
    double taken;

    for(size_t p = 0; p < passes; ++p)
    {
        particle_system ps;
        for(size_t i = 0; i < emitter_count; ++i)
        {
            switch(i % 3)
            {
                case 0:
                    ps.new_line_emitter(glm::vec2(100.0f * i, 100.0f * i), 1);
                    break;
                case 1:
                    ps.new_cone_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, M_PI/6);
                    break;
                case 2:
                    ps.new_area_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, 5);
                    break;
            }
        }

        int pcount;
        for(size_t i = 0; i < iterations; ++i)
        {
            start = now();
            ps.tick(1);
            taken = elapsed(start, now());

            pcount = ps.count();

            times[pcount] += taken;
            ++occurrences[pcount];

            // fprintf(stderr, "\ri: %d\tdt: %-20f", i, taken);

            // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
            if(i % period == 0)
                for(size_t j = 0; j < ps.e_positions.size(); ++j)
                    ps.e_emission_rates[j]++;
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
    double taken = 0.0;

    for(size_t p = 0; p < passes; ++p)
    {
        particle_system ps;

        start = now();
        for(size_t i = 0; i < emitter_count; ++i)
        {
            switch(i % 3)
            {
                case 0:
                    ps.new_line_emitter(glm::vec2(100.0f * i, 100.0f * i), 1);
                    break;
                case 1:
                    ps.new_cone_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, M_PI/6.0f);
                    break;
                case 2:
                    ps.new_area_emitter(glm::vec2(100.0f * i, 100.0f * i), 1, 5);
                    break;
            }
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
				fatal("tick test requires three arguments: <iterations> <increment period> <emitter count> <passes>");
            iterations = atoll(argv[optind++]);
            period = atoll(argv[optind++]);
			emitter_count = atoll(argv[optind++]);
            passes = atoll(argv[optind++]);
            run_measure_tick(iterations, period, emitter_count, passes);
			break;
		case INSERTION:
			if(optind != argc-2)
				fatal("insertion test requires two arguments: <emitter count> <passes>");
			emitter_count = atoll(argv[optind++]);
            passes = atoll(argv[optind++]);
            run_measure_insertion(emitter_count, passes);
			break;
	}

	return 0;
}
