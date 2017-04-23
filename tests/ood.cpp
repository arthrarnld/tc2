#include "02_ood_ecs.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>
#include <stdlib.h>

int main()
{
	srandom(time(NULL));

	world w;
	w.push_system(new health_system);
	w.push_system(new attack_system);
	w.push_system(new movement_system);

	auto & e1 = w.create();
	e1.add_component<health_component>(100, 1);
	e1.add_component<team_component>(1);
	e1.add_component<attack_component>(10,10,1);
	e1.add_component<position_component>(0, 0);

	auto & e2 = w.create();
	e2.add_component<health_component>(100, 1);
	e2.add_component<team_component>(2);
	e2.add_component<attack_component>(10,20,1);
	e2.add_component<position_component>(5, 0);

	while(w.get_entity_count() > 0)
	{
		w.update(1);
		log("%u", w.get_entity_count());
		usleep(200000);
	}
	return 0;
}
