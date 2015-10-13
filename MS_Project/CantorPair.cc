#include "CantorPair.h"
#include "Coordinates.h"
#include "AStarNode.h"

/* 
* Get the cantor pair of 3 points from a position 
* @param pos: Pointer to the position which should return a unique int
* @return a unique integer based on the parameter Position's two coordinates and depth
*/
int CantorPair::get_int(Position* pos)
{
	/* Special case when pos is a nullptr */
	if (pos == nullptr)
		return -1;

	/* Get the necessary variables from the position */
	int xcoord = pos->get_coord()->get_xcoord();
	int ycoord = pos->get_coord()->get_ycoord();
	int depth = pos->get_depth();

	/* Get the first cantor pair from the two coordinates */
	int first_cantor = (((xcoord + ycoord) * (xcoord + ycoord + 1)) / 2) + ycoord;
	return (((first_cantor + depth) * (first_cantor + depth + 1)) / 2) + depth;
}

/*
* Get the cantor pair of 2 positions
* @param pos_1: The first position in the cantor pair
* @param pos_2: The second position in the cantor pair
* @return a unique integer based on the two positions
*/
int CantorPair::get_int(Position* pos_1, Position* pos_2)
{
	/* Get the cantor pair from pos_1 */
	int cantor_1 = get_int(pos_1);

	/* Special case for when pos_2 is the parent and is a nullptr */
	if (pos_2 == nullptr)
		return -1 * cantor_1;

	/* Get the cantor pair from pos_2 */
	int cantor_2 = get_int(pos_2);

	/* Get the final cantor pair */
	return (((cantor_1 + cantor_2) * (cantor_1 + cantor_2 + 1)) / 2) + cantor_2;
}

/*
* Get the cantor pair from an A* Node, the position and the parent
* @param node: Pointer to the node which should return a unique int
* @return a unique integer based on the parameter
*/
int CantorPair::get_int(AStarNode* node)
{
	/* Get the cantor pair from the position */
	int cantor_1 = get_int(node->get_pos());

	/* If no parent exists, the hash value is -1 */
	if (node->get_parent() == nullptr)
		return -1 * cantor_1;
	
	/* Get the parents cantor pair */
	int cantor_2 = get_int(node->get_parent()->get_pos());

	/* Get the final cantor pair */
	return (((cantor_1 + cantor_2) * (cantor_1 + cantor_2 + 1)) / 2) + cantor_2;
}

/*
* Get the cantor pair of a coordinate and depth
* @param coord: The coordinate
* @param depth: The depth
* @return a unique integer based on the coordinate and depth
*/
int CantorPair::get_int(Coord* coord, int depth)
{
	/* Special case when pos is a nullptr */
	if (coord == nullptr)
		return -1;

	/* Get the necessary variables from the position */
	int xcoord = coord->get_xcoord();
	int ycoord = coord->get_ycoord();

	/* Get the cantor pair */
	int first_cantor = (((xcoord + ycoord) * (xcoord + ycoord + 1)) / 2) + ycoord;
	return (((first_cantor + depth) * (first_cantor + depth + 1)) / 2) + depth;
}