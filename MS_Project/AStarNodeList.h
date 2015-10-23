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

	/* Check if a node is in the list */
	AStarNode* check_duplicate(AStarNode* node);
	/* Check if a position is in the list */
	AStarNode* check_duplicate(Position* pos, Position* pos_parent);
	/* Check if a position is in the list given the hash */
	AStarNode* check_duplicate(int hash, int parent_hash);
	/* Add a node to the list */
	void add_node(AStarNode* add_node);
	/* Remove a parent from a node and delete it if it runs out of parents */
	bool delete_node(Position* pos, Position* parent_pos, bool del_mem);
	/* Copy by making new copies of each node in the parameter list */
	void node_copy(AStarNodeList* copy_list);
	/* 
	* Remove a reference to a node in the list without deleting or
	* decrementing the node's counter
 	*/
	void remove_hash(AStarNode* node);
	/* Print the list */
	void print_list();

	/* Operators */
	AStarNodeList & operator=(AStarNodeList& rhs);

	/* Accessors */
	std::unordered_map<int, AStarNode*, hash_struct>* get_list() { return &list; };
	AStarNodeList* get_parent() { return parent; };
	int get_size() { return list.size(); };

	/* Destructor */
	~AStarNodeList();
private:
	/* Hash table of all positions in this iteration of A* search */
	std::unordered_map<int, AStarNode*, hash_struct> list;
	/* Pointer to the parent ClosedList to avoid redundancy */
	AStarNodeList* parent;
};


#endif