#ifndef CLOSEDLIST_H
#define CLOSEDLIST_H

#include <unordered_map>

class Position;
class ClosedNode;

class ClosedList
{
public:
	/* Constructors */
	ClosedList();
	ClosedList(ClosedList* p_parent);

	/* Check if a position is in the list */
	bool check_duplicate(Position* pos);
	/* Check if a position is in the list given the hash */
	bool check_duplicate(int hash);
	/* Add a coordinate to the CLOSED list */
	void add_pos(ClosedNode* add_node);

	/* Destructor */
	~ClosedList();
private:
	/* Hash table of all positions in this iteration of A* search */
	std::unordered_map<int, ClosedNode*> list;
	/* Pointer to the parent ClosedList to avoid redundancy */
	ClosedList* parent;
};


#endif