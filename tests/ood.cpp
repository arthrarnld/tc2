#include "02_ood_ecs.hpp"
#include "common/log.hpp"
#include "common/time.hpp"

#include <unistd.h>

int main()
{
	world w;
	w.push_system(new health_system);
	auto & e = w.create();
	auto & hc = e.add_component<health_component>(100, 1);
	hc.set_health(50);

	while(w.get_entity_count() > 0)
	{
		w.update(1);
		log("%f", hc.get_health());
		sleep(1);
	}
	return 0;
}
