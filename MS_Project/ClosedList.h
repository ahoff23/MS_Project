#ifndef CLOSEDLIST_H
#define CLOSEDLIST_H

#include <unordered_map>

class Position;
class AStarNode;

class ClosedList
{
public:
	/* Constructors */
	ClosedList();
	ClosedList(ClosedList* p_parent);

	/* Check if a position is in the list */
	bool check_duplicate(AStarNode* node);
	/* Check if a position is in the list given the hash */
	bool check_duplicate(int hash);
	/* Add a node to the CLOSED list */
	void add_node(AStarNode* add_node);

	/* Destructor */
	~ClosedList();
private:
	/* Hash table of all positions in this iteration of A* search */
	std::unordered_map<int, AStarNode*> list;
	/* Pointer to the parent ClosedList to avoid redundancy */
	ClosedList* parent;
};


#endif