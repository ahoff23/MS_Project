#ifndef OPENNODE_H
#define OPENNODE_H

#include "Coordinates.h"

class ClosedNode;

/*
* A node saved on the OPEN list during A* search
*/
class OpenNode
{
public:
	/* Constructors */
	OpenNode();
	OpenNode(ClosedNode& p_parent, Position& p_pos);
	ClosedNode * get_parent() { return parent; };
	Position get_pos() { return pos; };
private:
	/* Pointer to a parent node in the CLOSED list */
	ClosedNode * parent;
	/* Position (coordinates and depth) of the node */
	Position pos;
};

#endif