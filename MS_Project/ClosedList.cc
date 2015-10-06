#include "ClosedList.h"
#include "Coordinates.h"
#include "ClosedNode.h"
#include "CantorPair.h"

/* 
* Default constructor 
*/
ClosedList::ClosedList()
{
	list = std::unordered_map<int, ClosedNode*>();
	parent = nullptr;
}

/* 
* Constructor with a parent as a parameter 
* @param parent: A pointer to the parent ClosedList that allows this
* ClosedList to reuse the positions on the old ClosedList.
*/
ClosedList::ClosedList(ClosedList* p_parent)
{
	list = std::unordered_map<int, ClosedNode*>();
	parent = p_parent;
}

/* 
* Check if a position is in the list 
* @param pos: The position to check the ClosedList for
* @return true if pos is found, false otherwise
*/
bool ClosedList::check_duplicate(Position* pos)
{
	/* Get the hash of the position */
	int hash = CantorPair::get_int(pos);

	/* Check this ClosedList first */
	if (list.find(hash) != list.end())
		return true;

	/* Make sure this ClosedList has a parent */
	if (parent == nullptr)
		return false;

	/* Recursively check all parents of this ClosedList */
	return parent->check_duplicate(hash);
}

/*
* Check if a position is in the list
* @param hash: The hash of the position being sought
* @return true if pos is found, false otherwise
*/
bool ClosedList::check_duplicate(int hash)
{
	/* Check this ClosedList first */
	if (list.find(hash) != list.end())
		return true;

	/* Make sure this ClosedList has a parent */
	if (parent == nullptr)
		return false;

	/* Recursively check all parents of this ClosedList */
	return parent->check_duplicate(hash);
}

/* 
* Add a ClosedNode to the ClosedList
* @param add_node: The ClosedNode to add
*/
void ClosedList::add_pos(ClosedNode* add_node)
{
	/* Make sure the add_node pointer is allocated */
	if (add_node == nullptr)
		return;

	/* Get the hash corresponding to this ClosedNode */
	int hash = CantorPair::get_int(add_node->get_pos());

	/* Place the ClosedNode in the hash table */
	list.emplace(hash, add_node);
}

/* Destructor */
ClosedList::~ClosedList()
{
	/* Delete all closed nodes in this ClosedList */
	for (auto it = list.begin(); it != list.end(); ++it)
		delete it->second;
}