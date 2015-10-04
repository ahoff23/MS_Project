#ifndef CLOSEDNODE_H
#define CLOSEDNODE_H

#include <unordered_map>
#include "Coordinates.h"

/*
* A node saved on the CLOSED list during A* search
*/
class ClosedNode
{
public:
	/* Constructors */
	ClosedNode();
	ClosedNode(Position& p_pos, ClosedNode* parent);
	Position get_pos() { return pos; };
private:
	/* Map from int (cantor pair of position values) to a pointer to a parent node */
	std::unordered_map<int, ClosedNode*> parents;
	/* Position (coordinates and depth) of the node*/
	Position pos;
};

#endif