#include <iostream>

#include "AStarNodeList.h"
#include "Coordinates.h"
#include "AStarNode.h"
#include "CantorPair.h"
#include "Exceptions.h"

/* 
* Default constructor 
*/
AStarNodeList::AStarNodeList()
{
	list = std::unordered_map<int, AStarNode*, hash_struct>();
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
	list = std::unordered_map<int, AStarNode*, hash_struct>();
	parent = p_parent;
}

/* 
* Check if a position is in the list 
* @param node: The A* node to check the list for
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(AStarNode* node)
{
	/* Make sure a node is passed */
	if (node == NULL)
		return NULL;

	/* Get the hash of the node */
	int hash = CantorPair::get_int(node);

	/* Get the hash of the node's parent */
	AStarNode* parent = node->get_parent();
	int parent_hash;
	if (parent == NULL)
		parent_hash = -1;
	else
		parent_hash = CantorPair::get_int(parent->get_pos());

	/* Check for duplicates based on the hash values */
	return check_duplicate(hash, parent_hash);
}

/* 
* Check if a position is in the list 
* @param pos: The position to check the list for
* @param pos_parent: The parent to check the list for
* (the hash is composed of both pos and parent)
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(Position* pos, Position* pos_parent)
{
	/* Get the hash of the position */
	int hash = CantorPair::get_int(pos);

	/* Get the hash of the node's parent */
	int parent_hash = CantorPair::get_int(pos_parent);
	
	/* Check for duplicates based on the hash values */
	return check_duplicate(hash, parent_hash);
}

/*
* Check if a position is in the list
* @param hash: The hash of the position being sought
* @param parent_hash: The hash of the position of this node's parent
* @return the A* Node Pointer if it is in the list, NULL otherwise
*/
AStarNode* AStarNodeList::check_duplicate(int hash, int parent_hash)
{
	/* Check this list for the position */
	auto it = list.find(hash);

	/* If the position is found in this list, check if its parent is as well */
	if (it != list.end())
	{
		std::unordered_map<int, std::pair<AStarNode*, int> >* parents = it->second->get_parents();
		auto it_2 = parents->find(parent_hash);

		/* Parent was not found */
		if (it_2 == parents->end())
			return NULL;

		/* Parent was found*/
		return it->second;
	}

	/* If this list has no parent, the node is not a duplicate */
	if (parent == NULL)
		return NULL;

	/* Recursively check all parents of this list */
	return parent->check_duplicate(hash, parent_hash);
}

/* 
* Add an AStarNode to the list
* @param add_node: The AStarNode to add
* @return true if a new node was added to the list, false if a node was simply editted
*/
bool AStarNodeList::add_node(AStarNode* add_node)
{
	/* Make sure the add_node pointer is allocated */
	if (add_node == NULL)
		return false;

	/* Get the hash corresponding to this AStarNode */
	int hash = CantorPair::get_int(add_node);

	/* Check if the node is already on the list */
	auto it = list.find(hash);

	/*
	* If the node is already on the list, just add the parent to the
	* list of parents in the node
	*/
	if (it != list.end())
	{
		it->second->add_parent(add_node->get_parent());
		return false;
	}
	else
	{
		/* Place the AStarNode in the hash table */
		list.emplace(hash, add_node);
		return true;
	}
}

/* 
* Remove a parent from a node and delete it if it runs out of parents
* @param node: The node to remove
* @param del_mem: True if the node should be deleted from memory, false otherwise
* @return true if the node is is deleted, false if it is found or found but not deleted
* (i.e. the counter is greater than 1)
*/
bool AStarNodeList::delete_node(Position* pos, Position* parent_pos, bool del_mem)
{
	/* Get the hash value of the node */
	int hash = CantorPair::get_int(pos);

	/* Find the node iterator */
	auto it = list.find(hash);

	/* Make sure the node exists */
	if (it == list.end())
		return false;

	/* Get the hash value of the node's parent */
	int parent_hash = CantorPair::get_int(parent_pos);

	/* Get the parent hash table */
	std::unordered_map<int, std::pair<AStarNode*, int> >* parents = it->second->get_parents();

	/* Find the parent iterator */
	auto parent_it = parents->find(parent_hash);

	/* Make sure the node exists */
	if (parent_it == parents->end())
		return false;

	/* Decrement the parent */
	bool erase = it->second->dec_parent(parent_it->second.first);

	/* If the node has no more parents */
	if (erase == false)
	{
		/* Delete the node pointed to by the list */
		if (del_mem == true)
			delete it->second;

		/* remove it from the list */
		list.erase(it);

		return true;
	}

	/* The node was found but not deleted */
	return false;
}

/*
* Remove a reference to a node in the list without deleting or
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
void AStarNodeList::node_copy(AStarNodeList* copy_list)
{
	/* Clear this list */
	list.clear();
	
	/* Copy and place each node into this list */
	for (auto it = copy_list->get_list()->begin(); it != copy_list->get_list()->end(); ++it)
	{
		AStarNode* new_node = new AStarNode(it->second);
		int hash = CantorPair::get_int(new_node);
		list.emplace(hash, new_node);
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
		std::cout << *it->second->get_pos()->get_coord() << " - " << it->second->get_pos()->get_depth();

		if (it->second->get_parent() == NULL)
			std::cout << " - no parent" << std::endl;
		else
			std::cout << " - " << *it->second->get_parent()->get_pos()->get_coord() << std::endl;
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