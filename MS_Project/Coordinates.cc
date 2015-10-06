#include "Coordinates.h"

/*
* Constructor with X and Y coordinates given as parameters
* @param x: The X coordinate of the coord
* @param y: The Y coordinate of the coord
*/
Coord::Coord(int x, int y)
{
	xcoord = x;
	ycoord = y;
}

/* 
* Default constructor for a Coord object
*/
Coord::Coord()
{
	/* Default values are impossible as long as negative coordinates are not allowed */
	xcoord = -1;
	ycoord = -1;
}

/*
* Equals operator
* @param rhs: The Coord object to copy into this object
* @return the object on the right hand side of the equal sign
*/
Coord & Coord::operator=(Coord& rhs)
{
	xcoord = rhs.get_xcoord();
	ycoord = rhs.get_ycoord();
	return rhs;
}

/* 
* Constructor with a coordinate and depth 
* @param p_coord: 2D coordinate (X and Y)
* @param p_depth: Depth of the coordinate in the agent's path
*/
Position::Position(Coord p_coord, int p_depth)
{
	coord = p_coord;
	depth = p_depth;
}


/*
* Constructor with two coordinates as ints and depth
* @param x_coord: X coordinate
* @param y_coord: Y coordinate
* @param p_depth: Depth of the coordinate in the agent's path
*/
Position::Position(int x_coord, int y_coord, int p_depth)
{
	coord = Coord(x_coord,y_coord);
	depth = p_depth;
}

/* Default constructor */
Position::Position()
{
	coord = Coord();

	/* Default value is an impossible value */
	depth = -1;
}

/*
* Equals operator
* @param rhs: The Position object to copy into this object
* @return the object on the right hand side of the equal sign
*/
Position & Position::operator=(Position& rhs)
{
	coord = *(rhs.get_coord());
	depth = rhs.get_depth();
	return rhs;
}