#include "CantorPair.h"
#include "Coordinates.h"
#include "AStarNode.h"

/* 
* Get the cantor pair of 3 points from a position 
* @param pos: Pointer to the position which should return a unique int
* @return a unique integer based on the parameter Position's 2 coordinates and depth
*/
int CantorPair::get_int(Position* pos)
{
	/* Get the necessary variables from the position */
	int xcoord = pos->get_coord()->get_xcoord();
	int ycoord = pos->get_coord()->get_ycoord();
	int depth = pos->get_depth();

	/* Get the first cantor pair from the two coordinates */
	int first_cantor = (((xcoord + ycoord) * (xcoord + ycoord + 1)) / 2) + ycoord;
	return (((first_cantor + depth) * (first_cantor + depth + 1)) / 2) + depth;
}

/*
* Get the cantor pair from an A* Node, the position and the parent
* @param node: Pointer to the node which should return a unique int
* @return a unique integer based on the parameter
*/
int CantorPair::get_int(AStarNode* node)
{
	/* Get the cantor pairing from both positions */
	int cantor_1 = get_int(node->get_pos());
	int cantor_2 = get_int(node->get_parent());

	/* Get the final cantor pair */
	return (((cantor_1 + cantor_2) * (cantor_1 + cantor_2 + 1)) / 2) + cantor_2;
}