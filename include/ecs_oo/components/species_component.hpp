#ifndef SPECIES_COMPONENT_HPP
#define SPECIES_COMPONENT_HPP

#include "base_component.hpp"

class species_component : public base_component
{
public:
	species_component(int id)
		: id(id)
	{  }

	int id;
};

#endif // SPECIES_COMPONENT_HPP
