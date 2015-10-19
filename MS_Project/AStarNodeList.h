#ifndef ASTARNODELIST_H
#define ASTARNODELIST_H

#include "UnorderedMap.h"

class Position;
class AStarNodePointer;
class AStarNode;

/*
* List of A* Nodes stored as a hash table. The hash
* is based on both the position of the A* Node and the
* position of the parent of the A* Node.
*/
class AStarNodeList
{
public:
	/* Constructors */
	AStarNodeList();
	AStarNodeList(AStarNodeList* p_parent);

	/* Check if a position is in the list */
	AStarNodePointer* check_duplicate(AStarNode* node);
	/* Check if a position is in the list */
	AStarNodePointer* check_duplicate(Position* pos, Position* pos_parent);
	/* Check if a position is in the list given the hash */
	AStarNodePointer* check_duplicate(int hash);
	/* Add a node to the list */
	void add_node(AStarNode* add_node);
	/*
	* Delete a node from the list if it exists (local list only).
	* This only decrements the nodes counter, and deletes it when the
	* counter reaches 0. This is done to ensure that a node is not deleted
	* if another path exists to reach that node.
	*/
	void delete_node(AStarNode* node, bool del_mem);
	/* Copy by making new copies of each node in the parameter list */
	void node_copy(AStarNodeList* copy_list);
	/* 
	* Remove a referenc to a node in the list without deleting or
	* decrementing the nodes counter
 	*/
	void remove_hash(AStarNode* node);

	/* Print the list */
	void print_list();

	/* Operators */
	AStarNodeList & operator=(AStarNodeList& rhs);

	/* Accessor */
	std::unordered_map<int, AStarNodePointer*, hash_struct>* get_list() { return &list; };
	AStarNodeList* get_parent() { return parent; };
	int get_size() { return list.size(); };

	/* Destructor */
	~AStarNodeList();
private:
	/* Hash table of all positions in this iteration of A* search */
	std::unordered_map<int, AStarNodePointer*, hash_struct> list;
	/* Pointer to the parent ClosedList to avoid redundancy */
	AStarNodeList* parent;
};


#endif