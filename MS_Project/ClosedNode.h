#ifndef CLOSEDNODE_H
#define CLOSEDNODE_H

#include <unordered_map>
#include "Coordinates.h"

class OpenNode;

/*
* A node saved on the CLOSED list during A* search
*/
class ClosedNode
{
public:
	/* Constructors */
	ClosedNode();
	ClosedNode(Position& p_pos, ClosedNode* parent, int p_cost);
	ClosedNode(OpenNode* open_node);

	/* Accessor functions */
	Position* get_pos() { return &pos; };
	int get_cost() { return cost; };
private:
	/* Map from int (cantor pair of position values) to a pointer to a parent node */
	std::unordered_map<int, ClosedNode*> parents;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/*Cost of this node*/
	int cost;
};

#endif