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
	OpenNode(ClosedNode* p_parent, Position& p_pos, int p_cost);

	/* Operator overloads */
	bool operator<(const OpenNode & comp) const;

	/* Get a pointer to the parent of this node from the CLOSED list */
	ClosedNode * get_parent() { return parent; };

	/* Get the position of this node */
	Position get_pos() { return pos; }; 

	/* Get the cost of the node */
	int get_cost() { return cost; };
private:
	/* Pointer to a parent node in the CLOSED list */
	ClosedNode * parent;
	/* Position (coordinates and depth) of the node */
	Position pos;
	/*Cost of this node*/
	int cost;
};

#endif