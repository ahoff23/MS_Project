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
}

/* 
* Check if a node is in the list 
* @param node: The A* node to check the list for
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(AStarNode* node)
{
	/* Make sure a node is passed as a parameter */
	if (node == NULL)
		throw TerminalException("Invalid node passed to check_duplicate().");

	/* A* Node found in the list */
	AStarNode* found;

	/* Having no parent means that the position must exist somewhere in the list */
	if (node->get_parent_bitmap() == 0)
		found = check_duplicate(node->get_pos(), NULL);
	/* Check for a duplicate with a parent */
	else
	{
		Coord parent_coord = node->get_parent();
		found = check_duplicate(node->get_pos(), &parent_coord);
	}

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

	/* Check the node for the parent */
	if (found->check_parent(parent_coord) == false)
		return NULL;
	return found;
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
	return list->find(pos);
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

	/* If the node has no more parents after the deletion of this parent */
	if (found->del_parent(parent_coord) == 0)
	{


	/*	Position check = Position(0, 5, 5);
		if (check == *pos)
		{
			std::cout << "PRE DELETION" << std::endl;
			print_list();
		}*/


		/* remove it from the list */
		list->erase(pos);



		/* Delete the node pointed to by the list or mark it for deletion */
		if (del_mem == true)
			delete found;
		else
			found->mark_for_deletion();

	
		/*
		if (check == *pos)
		{
			std::cout << "POST DELETION" << std::endl;
			print_list();
		}
		*/


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
* Search the map as a linked list for a specific Position
* @param pos: The position to search the list for
*/
AStarNode* AStarNodeList::search_node(Position* pos)
{
	return list->search_node(pos);
}

/*
* Print the list 
*/
void AStarNodeList::print_list() 
{ 
	list->print_map(); 
}

/*
* Destructor
*/
AStarNodeList::~AStarNodeList()
{
	delete list;
}