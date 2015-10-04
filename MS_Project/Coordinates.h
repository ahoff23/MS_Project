#ifndef COORDINATES_H
#define COORDINATES_H

/*
* Denotes a coordinate in a 2D grid system with an x and y coordinate
*/
class Coord
{
public:
	/* Declare constructors */
	Coord(int x, int y);
	Coord();

	/* Operator overloads */
	Coord & operator=(Coord& rhs);

	/* Access functions */
	int get_xcoord() { return xcoord; };
	int get_ycoord() { return ycoord; };
private:
	/* 
	* X and Y coordinates which are assumed to be non-netaive.
	* If negative numbers are allowed, then the default value is not a 
	* protected value.
	*/
	int xcoord;
	int ycoord;
};

/*
* Struct that maintains an agent's location in a 2D grid system at a given depth
* in the agent's path.
*/
class Position
{
public:
	/* Constructor with a coordinate and depth */
	Position(Coord p_coord, int p_depth);
	Position();

	/* Operator overloads */
	Position & operator=(Position& rhs);

	/* Access functions */
	Coord * get_coord() { return &coord; };
	int get_depth() { return depth; };
private:
	Coord coord;
	int depth;
};

#endif