#include "World.h"
#include "Coordinates.h"

World::World()
{
	/* Default values not possible values for error checking */
	coords = nullptr;
	max_x = -1;
	max_y = -1;
}

World::World(std::string txt_file)
{

}