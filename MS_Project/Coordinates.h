#ifndef COORDINATES_H
#define COORDINATES_H

#include <fstream>

/*
* Denotes a coordinate in a 2D grid system with an x and y coordinate
*/
class Coord
{
public:
	/* Declare constructors */
	Coord(unsigned short x, unsigned short y);
	Coord(Coord* copy_coord);
	Coord();

	/* Operator overloads */
	Coord & operator=(Coord& rhs);
	bool operator!=(Coord& rhs);
	friend std::ostream & operator<<(std::ostream& out, Coord& coord);
	bool operator==(Coord& rhs);

	/* Setter functions */
	void set_x(unsigned short x) { xcoord = x; };
	void set_y(unsigned short y) { ycoord = y; };

	/* Access functions */
	unsigned short get_xcoord() const { return xcoord; };
	unsigned short get_ycoord() const { return ycoord; };

	/* Get a value for comparing this object to another (in MultiMap.cpp) */
	Coord* get_comp() { return this; };
private:
	/* 
	* X and Y coordinates which are assumed to be non-netaive.
	* If negative numbers are allowed, then the default value is not a 
	* protected value.
	*/
	unsigned short xcoord;
	unsigned short ycoord;
};

/*
* Struct that maintains an agent's location in a 2D grid system at a given depth
* in the agent's path.
*/
class Position
{
public:
	/* Constructor with a coordinate and depth */
	Position(Coord p_coord, unsigned short p_depth);
	Position(unsigned short x_coord, unsigned short y_coord, unsigned short p_depth);
	Position();

	/* Operator overloads */
	Position & operator=(Position& rhs);
	bool operator==(Position& rhs);
	friend std::ostream& operator<<(std::ostream& out, Position& pos);

	/* Set functions */
	void set_x(unsigned short x) { coord.set_x(x); };
	void set_y(unsigned short y) { coord.set_y(y); };
	void set_depth(unsigned short d) { depth = d; };

	/* Access functions */
	Coord * get_coord() { return &coord; };
	unsigned short get_depth() const { return depth; };
	unsigned short get_x_coord() const { return coord.get_xcoord(); };
	unsigned short get_y_coord() const { return coord.get_ycoord(); };
private:
	Coord coord;
	unsigned short depth;
};

/* Class containing a Position and the agent in that Position */
class AgentPos
{
public:
	/* Constructor */
	AgentPos(int p_agent_num, Position* p_pos);
	
	/* Accessors */
	int get_agent_num() { return agent_num; };
	Position* get_pos() { return &pos; };
private:
	/* The agent's index number */
	int agent_num;
	/* The position the agent is located in */
	Position pos;
};

#endif