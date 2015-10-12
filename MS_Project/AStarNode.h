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
	AStarNode(Position p_pos, AStarNode* p_parent, float p_cost);
	AStarNode(AStarNode* a_star_node);

	/* Operator overloads */
	bool operator<(const AStarNode& comp) const;
	bool operator>(const AStarNode& comp) const;

	/* Accessor functions */
	Position* get_pos() { return &pos; };
	float get_cost() { return cost; };
	int get_point_count() { return point_count; };

	/* Increment and decrement the point count */
	void inc_point_count() { point_count++; };
	void dec_point_count() { point_count--; };

	/* Return the first parent */
	AStarNode * get_parent() { return parent; };
private:
	/* Parent node of the agent */
	AStarNode* parent;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/* Cost of this node */
	float cost;
	/* Number of paths that lead to this node */
	int point_count;
};

#endif