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
	AStarNode(Position* p_pos, AStarNode* parent, float p_cost);
	AStarNode(AStarNode* open_node);

	/* Add a parent to the hash table of parents if it is not already there */
	void add_parent(AStarNode* new_parent);

	/* Get a pointer to a parent of this node from the CLOSED list */
	AStarNode * get_parent() { return parents.begin()->second; };

	/* Operator overloads */
	bool operator<(const AStarNode& comp) const;
	bool operator>(const AStarNode& comp) const;

	/* Accessor functions */
	Position* get_pos() { return &pos; };
	float get_cost() { return cost; };
	int get_parent_count() { return parent_count; };
private:
	/* Map from int (cantor pair of position values) to a pointer to a parent node */
	std::unordered_map<int, AStarNode*> parents;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/* Cost of this node */
	float cost;
	/* Number of parents pointing to this node */
	int parent_count;
};

#endif