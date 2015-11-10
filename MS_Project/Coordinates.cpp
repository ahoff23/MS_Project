#include <string>

#include "Coordinates.h"
#include "Exceptions.h"
#include "Macros.h"

/*
* Constructor with X and Y coordinates given as parameters
* @param x: The X coordinate of the coord
* @param y: The Y coordinate of the coord
*/
Coord::Coord(unsigned short x, unsigned short y)
{
	xcoord = x;
	ycoord = y;
}

/*
* Coord copy constructor
* @param copy_coord: The coordinate to copy
*/
Coord::Coord(Coord* copy_coord)
{
	xcoord = copy_coord->get_xcoord();
	ycoord = copy_coord->get_ycoord();
}

/* 
* Default constructor for a Coord object
*/
Coord::Coord()
{
	/* Default values (overflow because coordinates are unsigned) */
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
* Equals comparison operator
* @param rhs: The Coord object to compare
* @return true if rhs equals this coordinate, false otherwise
*/
bool Coord::operator==(Coord& rhs)
{
	/* Return true if both Coords have the same X and Y coords */
	if (xcoord == rhs.get_xcoord() && ycoord == rhs.get_ycoord())
		return true;
	return false;
}

/*
* Output stream operator
* @param out: The Coord to output
* @return ostream containing data to be outputted to console
*/
std::ostream& operator<<(std::ostream& out, Coord& coord)
{
	out << "(" << coord.get_xcoord() << "," << coord.get_ycoord() << ")";
	return out;
}

/*
* Compares the X and Y coordinates of both Coords
* @return true if both Coords have the same X and Y coordinates
*/
bool Coord::operator!=(Coord& rhs)
{
	if (xcoord != rhs.get_xcoord() || ycoord != rhs.get_ycoord())
		return true;

	return false;
}

/* 
* Constructor with a coordinate and depth 
* @param p_coord: 2D coordinate (X and Y)
* @param p_depth: Depth of the coordinate in the agent's path
*/
Position::Position(Coord p_coord, unsigned short p_depth)
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
Position::Position(
	unsigned short x_coord, unsigned short y_coord, unsigned short p_depth
	) : coord(x_coord, y_coord)
{
#ifdef SEARCH_DEPTH
	if (p_depth > SEARCH_DEPTH)
		throw TerminalException("Exceeded max search depth.");
#endif

	depth = p_depth;
}

/* Default constructor */
Position::Position() : coord()
{
	/* Default value (overflow because depth is unsigned) */
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

/*
* Equals comparison operator
* @param rhs: The Position object to compare
* @return true if rhs equals this position, false otherwise
*/
bool Position::operator==(Position& rhs)
{
	/* Get the Coord object of rhs */
	Coord rhs_coord = rhs.get_coord();

	/* Test if the 3 parameters of the Position objects are the same */
	if (
		coord.get_xcoord() == rhs_coord.get_xcoord() &&
		coord.get_ycoord() == rhs_coord.get_ycoord() &&
		depth == rhs.get_depth()
		)
		return true;

	return false;
}

/*
* Output stream operator
* @param out: The Position to output
* @return ostream containing data to be outputted to console
*/
std::ostream& operator<<(std::ostream& out, Position& pos)
{
	out << "(" << pos.get_coord()->get_xcoord() << "," << pos.get_coord()->get_ycoord() << 
		") at depth " << pos.get_depth();
	return out;
}

/*
* Constructor 
*/
AgentPos::AgentPos(int p_agent_num, Position* p_pos)
{
	agent_num = p_agent_num;
	pos = *p_pos;
}