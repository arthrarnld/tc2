#include "systems/reproduction.hpp"
#include "common/log.hpp"
#include "world.hpp"

#include "components/movement.hpp"

bool update_reproduction(world * w, double dt)
{
	reproduction & r = w->rep;
	movement & m = w->mov;

	#ifdef DO_PARTITION_ARRAYS
		// Idle
		for(size_t i = 0; i < r.partitions[0]; ++i)
			r.desire[i] += r.libido[i] * dt;
		// Mating
		for(size_t i = r.partitions[0]; i < r.size(); )
		{
			r.desire[i] -= 25.0f;
			if(r.desire[i] <= 0)
			{
				r.desire[i] = 0;
				m.stop_seeking_mate(m.lookup(r.owner[i]));
				debug("%llu done mating", r.owner[i]);
				i = r.stop_mating(i);
			}
			else ++i;
		}

	#else

		for(size_t i = 0; i < r.size(); ++i)
		{
			if(r.state[i] == reproduction::MATING) {
				r.desire[i] -= 25.0f;
				if(r.desire[i] <= 0) {
					r.desire[i] = 0;
					r.stop_mating(i);
					m.stop_seeking_mate(m.lookup(r.owner[i]));
					debug("%llu done mating", r.owner[i]);
				}
			} else r.desire[i] += r.libido[i] * dt;
		}

	#endif
	return true;
}
