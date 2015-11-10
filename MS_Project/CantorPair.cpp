#include <cmath>

#include "CantorPair.h"
#include "Coordinates.h"
#include "AStarNode.h"
#include "Exceptions.h"

/* 
* Get the cantor pair of 3 points from a position 
* @param pos: Pointer to the position which should return a unique int
* @return a unique integer based on the parameter Position's two coordinates and depth
*/
unsigned int CantorPair::get_int(Position* pos)
{
	/* Special case when pos is a NULL */
	if (pos == NULL)
		throw TerminalException("Null position for cantor pair function.");

	/* Get the necessary variables from the position */
	unsigned short xcoord = pos->get_coord()->get_xcoord();
	unsigned short ycoord = pos->get_coord()->get_ycoord();
	unsigned short depth = pos->get_depth();

	/* Get the first cantor pair from the two coordinates */
	unsigned long first_cantor = (((xcoord + ycoord) * (xcoord + ycoord + 1)) / 2) + ycoord;
	unsigned long cantor = (((first_cantor + depth) * (first_cantor + depth + 1)) / 2) + depth;
	return cantor;
}

/*
* Get the cantor pair of 2 positions
* @param pos_1: The first position in the cantor pair
* @param pos_2: The second position in the cantor pair
* @return a unique integer based on the two positions
*/
unsigned int CantorPair::get_int(Position* pos_1, Position* pos_2)
{
	/* Get the cantor pair from pos_1 */
	unsigned long cantor_1 = get_int(pos_1);

	/* Special case for when pos_2 is the parent and is a NULL */
	if (pos_2 == NULL)
		throw TerminalException("Null second position for cantor pair function.");

	/* Get the cantor pair from pos_2 */
	unsigned long cantor_2 = get_int(pos_2);

	/* Get the final cantor pair */
	unsigned long cantor = (((cantor_1 + cantor_2) * (cantor_1 + cantor_2 + 1)) / 2) + cantor_2;
	return cantor;
}

/*
* Get the cantor pair from an A* Node's position
* @param node: Pointer to the node which should return a unique int
* @return a unique integer based on the parameter
*/
unsigned int CantorPair::get_int(AStarNode* node)
{
	/* Make sure the node is not NULL */
	if (node == NULL)
		throw TerminalException("Null A* Node for cantor pair function.");

	/* Get the cantor pair based on the node's position */
	return get_int(node->get_pos());
}

/*
* Get the cantor pair of a coordinate and depth
* @param coord: The coordinate
* @param depth: The depth
* @return a unique integer based on the coordinate and depth
*/
unsigned int CantorPair::get_int(Coord* coord, int depth)
{
	/* Special case when the coordinate is a NULL */
	if (coord == NULL)
		throw TerminalException("Null Coord for cantor pair function.");

	/* Get the necessary variables from the coordinate */
	int xcoord = coord->get_xcoord();
	int ycoord = coord->get_ycoord();

	/* Get the cantor pair */
	unsigned long first_cantor = (((xcoord + ycoord) * (xcoord + ycoord + 1)) / 2) + ycoord;
	unsigned long cantor = (((first_cantor + depth) * (first_cantor + depth + 1)) / 2) + depth;
	return cantor;
}

/*
* Get the cantor pair of a coordinate
* @param coord: The coordinate
* @return a unique integer based on the coordinate
*/
unsigned int CantorPair::get_int(Coord* coord)
{
	/* Special case when the coordinate is a NULL */
	if (coord == NULL)
		return -1;

	/* Get the necessary variables from the coordinate */
	int xcoord = coord->get_xcoord();
	int ycoord = coord->get_ycoord();

	unsigned long cantor = ((xcoord + ycoord) * (xcoord + ycoord + 1) / 2) + ycoord;
	return cantor;
}