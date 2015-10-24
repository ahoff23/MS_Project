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
	AStarNode * get_parent();
	int get_parent_count() { return parents.begin()->second.second; };
	std::unordered_map<int, std::pair<AStarNode*, int> >* get_parents() { return &parents; };

	/* Add a parent if it is not already in the parents table */
	bool add_parent(AStarNode* parent);
	/* Remove a parent node (decrement the counter) */
	bool dec_parent(AStarNode* parent);

private:
	/* Parent node of the agent */
	std::unordered_map<int,std::pair<AStarNode*,int> > parents;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/* Cost of this node */
	float cost;
};

#endif