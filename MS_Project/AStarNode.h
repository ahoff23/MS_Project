#ifndef ASTARNODE_H
#define ASTARNODE_H

#include <unordered_map>
#include "Coordinates.h"

/*
* A node saved on the CLOSED list during A* search
*/
class AStarNode
{
public:
	/* Constructors */
	AStarNode();
	AStarNode(Position* p_pos, AStarNode* p_parent, float p_cost);
	AStarNode(AStarNode* open_node);

	/* Operator overloads */
	bool operator<(const AStarNode& comp) const;
	bool operator>(const AStarNode& comp) const;

	/* Accessor functions */
	Position* get_pos() { return &pos; };
	float get_cost() { return cost; };
	AStarNode * get_parent() { return parent; };
private:
	/* Parent node of the agent */
	AStarNode* parent;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/* Cost of this node */
	float cost;
};

#endif