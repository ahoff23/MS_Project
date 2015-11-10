#ifndef ASTARNODE_H
#define ASTARNODE_H

#include <unordered_map>

#include "Coordinates.h"

class Position;
class Coord;

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
	float get_cost() const { return cost; };
	Coord* get_parent();
	std::unordered_map<unsigned int, Coord>* get_parents() { return &parents; };
	bool get_del_mark() const { return del_mark; };

	/* Add a parent if it is not already in the parents table */
	void add_parent(AStarNode* parent);
	/* Remove a parent node (decrement the counter) */
	void del_parent(Coord* parent);
	/* Mark for deletion */
	void mark_for_deletion() { del_mark = true; };
	/* Print parents */
	void print_parents();

















	static int pub_count;


private:
	/* Parent node of the agent */
	std::unordered_map<unsigned int,Coord> parents;
	/* Position (coordinates and depth) of the node*/
	Position pos;
	/* Cost of this node */
	float cost;
	/* Mark this node for deletion if it is in the OPEN list */
	bool del_mark;












	int count;

};

#endif