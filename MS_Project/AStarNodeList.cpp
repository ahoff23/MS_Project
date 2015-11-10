#include <iostream>

#include "AStarNodeMultiMap.h"
#include "AStarNodeList.h"
#include "Coordinates.h"
#include "AStarNode.h"
#include "Exceptions.h"
#include "HashStruct.h"

/* 
* Default constructor 
*/
AStarNodeList::AStarNodeList()
{
	list = new AStarNodeMultiMap();
	parent = NULL;
}

/*
* Constructor with a parent as a parameter
* @param p_parent: A pointer to the parent list that allows this list
* to reuse the positions on the parent list i.e. the ancestor lists
* of this lists are considered extensions of this list. This way, when
* checking for a duplicate, this list and all of its ancestors will be
* checked.
*/
AStarNodeList::AStarNodeList(AStarNodeList* p_parent)
{
	list = new AStarNodeMultiMap();
	parent = p_parent;
}

/* 
* Check if a node is in the list 
* @param node: The A* node to check the list for
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(AStarNode* node)
{
	/* Make sure a node is passed */
	if (node == NULL)
		return NULL;

	/* Get the node's parent */
	Coord* parent = node->get_parent();

	/* A* Node found in the list */
	AStarNode* found;

	/* Having no parent means that the position must exist somewhere in the list */
	if (parent == NULL)
		found = check_duplicate(node->get_pos(), NULL);
	/* Check for a duplicate with a parent */
	else
		found = check_duplicate(node->get_pos(), parent);

	/* Make sure the returned node is the SAME node pointed to by the A* Node parameter */
	if (found == NULL || node == found)
		return found;

	/*
	* If the found node is not NULL and not the same node pointed to as the parameter,
	* an error has occurred
	*/
	throw TerminalException(
		"Found node is not a pointer to the same node as the node being sought."
		);
}

/* 
* Check if a position is in the list 
* @param pos: The position to check the list for
* @param parent_coord: The parent to check the list for
* @return the A* Node pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(Position* pos, Coord* parent_coord)
{
	/* Find the node in the list based on the position only */
	AStarNode* found = list->find(pos);

	/* If the node is found search for its parent */
	if (found != NULL)
	{
		/* Get the node's list of parents */
		std::unordered_map<unsigned int, Coord>* parents = found->get_parents();

		/* If there is no parent, make sure the parent list's size is zero and return the node */
		if (parent_coord == NULL)
		{
			if (parents->size() == 0)
				return found;

			/* If the list has parents, throw an error */
			throw TerminalException("Node searched for without a parent has parents.");
		}

		/* Search for the parent coordinate in the list of parents */
		auto it = parents->find(HashStruct::hash_coord(parent_coord));

		/* Parent was not found */
		if (it == parents->end())
			return NULL;

		/* Parent was found*/
		return found;
	}

	/* If this list has no parent, the node is not a duplicate */
	if (parent == NULL)
		return NULL;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(pos, parent_coord);
}

/*
* Check if a position is in the list given the hash 
* (this function does not consider the parent)
* @param pos: The position being searched for
* @return the node being searched for or NULL if the node is not found
*/
AStarNode* AStarNodeList::check_duplicate(Position* pos)
{
	/* Find the node in the list based on the position only */
	AStarNode* found = list->find(pos);

	/* Return NULL if the node is not found */
	if (found != NULL)
		return found;

	/* If this list has no parent, the node is not a duplicate */
	if (parent == NULL)
		return NULL;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(pos);
}

/* 
* Add an AStarNode to the list
* @param add_node: The AStarNode to add
*/
void AStarNodeList::add_node(AStarNode* add_node)
{
	/* Make sure the add_node is allocated */
	if (add_node == NULL)
		throw TerminalException("NULL node added to AStarNodeList.");

	/* Make sure the node is not already in the list */
	AStarNode* found = list->find(add_node->get_pos());
	if (found != NULL)
		throw TerminalException("Pre-existing node added to AStarNodeList.");

	/* Add the node to the list */
	list->emplace(add_node);
}

/* 
* Remove a parent from a node and delete it if it runs out of parents
* @param pos: The position of the node to remove
* @param parent_pos: The parent of the position to remove
* (only remove a position/parent combo)
* @param del_mem: True if the node should be deleted here, false if it should
* simply be marked for deletion (so the OPEN list heap does not contain deleted nodes)
* @return 0 if the node is not found, 
* 1 if it is found but not deleted, 2 if it is found and deleted
*/
int AStarNodeList::delete_node(Position* pos, Coord* parent_coord, bool del_mem)
{
	/* Find the node iterator */
	AStarNode* found = list->find(pos);

	/* Make sure the node exists */
	if (found == NULL)
		return 0;
	
	/* Decrement the parent */
	found->del_parent(parent_coord);

	/* If the node has no more parents after the deletion of this parent */
	if (found->get_parents()->size() == 0)
	{
		/* Delete the node pointed to by the list or mark it for deletion */
		if (del_mem == true)
			delete found;
		else
			found->mark_for_deletion();

		/* remove it from the list */
		list->erase(pos);

		return 2;
	}

	/* The node was found but not deleted */
	return 1;
}

/*
* Remove a reference to a node in the list without deleting or
* decrementing the node's counter
* @param node: The node to remove
*/
void AStarNodeList::remove_hash(AStarNode* node)
{
	/* Remove the node from the list */
	list->erase(node->get_pos());
}

/*
* Remove a reference to a node in the list without deleting or
* decrementing the node's counter
* @param pos: The position of the node to remove
*/
void AStarNodeList::remove_hash(Position* pos)
{
	/* Remove the node from the list */
	list->erase(pos);
}

/* 
* Copy by making new copies of each node in the parameter list 
* @param copy_list: List to copy
*/
void AStarNodeList::node_copy(AStarNodeList* copy_list)
{
	/* Clear this list */
	list->clear();
	copy_list->get_list()->node_copy(list);
}

/*
* Place all elements in the list into a heap 
* @param heap: The heap to place each element from this list into
*/
void AStarNodeList::heap_place(std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >* heap)
{
	list->heap_place(heap);
}

/*
* Get the size of the multimap
* @return the size of the multimap
*/
int AStarNodeList::get_size() const
{
	return list->size();
}

/*
* Destructor
*/
AStarNodeList::~AStarNodeList()
{
	delete list;
}