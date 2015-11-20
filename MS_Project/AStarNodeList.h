#ifndef ASTARNODELIST_H
#define ASTARNODELIST_H

#include <queue>
#include <functional>

class AStarNodeMultiMap;
class Position;
class AStarNodePointer;
class AStarNode;
class Coord;

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
	/* Check if a position is in the list with its parent */
	AStarNode* check_duplicate(Position* pos, Coord* parent_coord);
	/* Check if a position is in the list independent of it parent */
	AStarNode* check_duplicate(Position* pos);
	/* Add a node to the list */
	void add_node(AStarNode* add_node);
	/* Remove a parent from a node and delete it if it runs out of parents */
	int delete_node(Position* pos, Coord* parent_coord, bool del_mem);
	/* Copy by making new copies of each node in the parameter list */
	void node_copy(AStarNodeList* copy_list);
	/* 
	* Remove a reference to a node in the list without deleting or
	* decrementing the node's counter
 	*/
	void remove_hash(AStarNode* node);
	void remove_hash(Position* pos);
	/* Place all elements in the list into a heap */
	void heap_place(std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >* heap);
	/* Search the map as a linked list for a specific Position */
	AStarNode* search_node(Position* pos);

	/* Accessors */
	AStarNodeMultiMap* get_list() { return list; };
	AStarNodeList* get_parent() const { return parent; };
	int get_size() const;

	/* Destructor */
	~AStarNodeList();
private:
	/* Hash table of all positions in this iteration of A* search */
	AStarNodeMultiMap* list;
	/* Pointer to the parent ClosedList to avoid redundancy */
	AStarNodeList* parent;
};


#endif