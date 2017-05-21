#include "part_oo.hpp"
#include "common/log.hpp"
#include "common/time.hpp"
#include <fstream>
#include <unordered_map>

const int ITERATIONS = 100000;
const int INCREASE = 1000;

std::unordered_map<int, double> times;
std::unordered_map<int, int> occurrences;

int main()
{
    particle_emitter e;
    time_point start;
    double taken;

    for(int i = 0; i < ITERATIONS; ++i)
    {
        start = now();
        e.tick(1);
        taken = elapsed(start, now());

        printf("\ri: %d\tdt: %-20f", i, taken);

        times[e.get_particle_count()] += taken;
        ++occurrences[e.get_particle_count()];
        // log("particle count: %d\t time taken: %f", e.get_particle_count(), taken);
        if(i % INCREASE == 0)
        {
            e.set_emission_rate(e.get_emission_rate() + 10);
        }
    }

    std::ofstream file("out_part_oo");
    for(auto & p : times)
    {
        file << p.first << '\t' << p.second / occurrences[p.first] << '\n';
    }
}
