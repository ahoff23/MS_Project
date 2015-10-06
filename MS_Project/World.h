#ifndef WORLD_H
#define WORLD_H

#include <string>

class Coord;

class World
{
public:
	World();
	/* Constructor creates the world by reading from a text file */
	World(std::string txt_file);
	/* Check if a coordinate is open and that it exists */
	bool check_coord(Coord* coord);
	~World();
private:
	/* Boolean matrix where true means the space is open, false if it is blocked */
	bool * coords;
	/* Maximum X coordinate of the matrix (starts at 0) */
	int max_x;
	/* Maximum Y coordinate of the matrix(starts at 0) */
	int max_y;
};

#endif