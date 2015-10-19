#include "AStarNodeList.h"
#include "Coordinates.h"
#include "AStarNode.h"
#include "AStarNodePointer.h"
#include "CantorPair.h"

/* 
* Default constructor 
*/
AStarNodeList::AStarNodeList()
{
	list = std::unordered_map<int, AStarNodePointer*, hash_struct>();
	parent = NULL;
}

/* 
* Constructor with a parent as a parameter 
* @param parent: A pointer to the parent list that allows this list
* to reuse the positions on the parent list i.e. the ancestor lists
* of this lists are considered extensions of this list. This way, when
* checking for a duplicate, this list and all of its ancestors will be
* checked.
*/
AStarNodeList::AStarNodeList(AStarNodeList* p_parent)
{
	list = std::unordered_map<int, AStarNodePointer*, hash_struct>();
	parent = p_parent;
}

/* 
* Check if a position is in the list 
* @param node: The A* node to check the list for
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNodePointer* AStarNodeList::check_duplicate(AStarNode* node)
{
	/* Get the hash of the node */
	int hash = CantorPair::get_int(node);

	/* Check this list first */
	auto it = list.find(hash);
	if (it != list.end())
		return it->second;

	/* Make sure this list has a parent */
	if (parent == NULL)
		return false;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(hash);
}

/* 
* Check if a position is in the list 
* @param pos: The position to check the list for
* @param pos_parent: The parent to check the list for
* (the hash is composed of both pos and parent)
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNodePointer* AStarNodeList::check_duplicate(Position* pos, Position* pos_parent)
{
	/* Get the hash of the positions */
	int hash = CantorPair::get_int(pos, pos_parent);

	/* Check this list first */
	auto it = list.find(hash);
	if (it != list.end())
		return it->second;

	/* Make sure this list has a parent */
	if (parent == NULL)
		return NULL;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(hash);
}

/*
* Check if a position is in the list
* @param hash: The hash of the position being sought
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNodePointer* AStarNodeList::check_duplicate(int hash)
{
	/* Check this list first */
	auto it = list.find(hash);
	if (it != list.end())
		return it->second;

	/* Make sure this list has a parent */
	if (parent == NULL)
		return NULL;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(hash);
}

/* 
* Add an AStarNode to the list
* @param add_node: The AStarNode to add
*/
void AStarNodeList::add_node(AStarNode* add_node)
{
	/* Make sure the add_node pointer is allocated */
	if (add_node == NULL)
		return;

	/* Get the hash corresponding to this AStarNode */
	int hash = CantorPair::get_int(add_node);

	/* Create a new A* Node Pointer */
	AStarNodePointer* ptr = new AStarNodePointer(add_node);

	/* Place the AStarNode in the hash table */
	list.emplace(hash, ptr);
}

/* 
* Remove a node from the list if it exists (local list only)
* @param node: The node to remove
* @param del_mem: True if the node should be deleted from memory, false otherwise
*/
void AStarNodeList::delete_node(AStarNode* node, bool del_mem)
{
	/* Get the hash value of the node */
	int hash = CantorPair::get_int(node);

	/* Find the node iterator */
	auto it = list.find(hash);

	/* Make sure the node exists */
	if (it == list.end())
		return;

	/* Get the node */
	AStarNodePointer* ptr = it->second;

	/* Decrement the node counter */
	ptr->dec();
	
	/* If the pointer counter reaches 0, remove from the list */
	if (ptr->get_counter() == 0)
	{
		/* Delete the node pointed to by the list */
		if (del_mem == true)
			delete it->second;

		list.erase(it);
	}
}


/*
* Remove a referenc to a node in the list without deleting or
* decrementing the nodes counter
* @param node: The node to remove
*/
void AStarNodeList::remove_hash(AStarNode* node)
{
	/* Get the has value of the node */
	int hash = CantorPair::get_int(node);

	/* Remove the node from the list */
	list.erase(hash);
}

/* 
* Copy an existing AStarNodeList into this one 
* @param rhs: The AStarNodeList to copy
*/
AStarNodeList & AStarNodeList::operator=(AStarNodeList& rhs)
{
	list = *(rhs.get_list());
	parent = rhs.get_parent();

	return rhs;
}

/* 
* Copy by making new copies of each node in the parameter list 
* @param copy_list: List to copy
*/
#include <iostream>
void AStarNodeList::node_copy(AStarNodeList* copy_list)
{
	/* Clear this list */
	list.clear();

	/* Copy and place each node into this list */
	for (auto it = copy_list->get_list()->begin(); it != copy_list->get_list()->end(); ++it)
	{
		AStarNode* new_node = new AStarNode(it->second->get_ptr());
		AStarNodePointer* new_node_ptr = new AStarNodePointer(new_node);
		new_node_ptr->set_counter(it->second->get_counter());
		int hash = CantorPair::get_int(new_node);
		list.emplace(hash, new_node_ptr);
	}
}

/*
* Print the list 
*/
void AStarNodeList::print_list()
{
	std::cout << "PRINTING LIST" << std::endl;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		std::cout << *it->second->get_ptr()->get_pos()->get_coord() << " - " << it->second->get_ptr()->get_pos()->get_depth();

		if (it->second->get_ptr()->get_parent() == NULL)
			std::cout << " - no parent" << std::endl;
		else
			std::cout << " - " << *it->second->get_ptr()->get_parent()->get_pos()->get_coord() << std::endl;
	}
}


/* 
* Destructor 
*/
AStarNodeList::~AStarNodeList()
{
	/* Delete all closed nodes in this ClosedList */
	for (auto it = list.begin(); it != list.end(); ++it)
		delete it->second;
}