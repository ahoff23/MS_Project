#include <iostream>

#include "AStarNode.h"
#include "Exceptions.h"
#include "HashStruct.h"

/*
* Default constructor
*/
AStarNode::AStarNode()
{
	/* Set the position as the default position */
	Position* temp = new Position();
	pos = *temp;
	delete temp;

	/* No parent for this node */
	parents = std::unordered_map<unsigned int, Coord>();
	parents_ptr = &parents;
	parents_copied = true;

	/* 
	* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;

	/* Not marked for deletion by default */
	del_mark = false;
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param p_parent: Parent of this node, NULL if no parent exists
* @param p_cost: The cost of the node
*/
AStarNode::AStarNode(Position p_pos, AStarNode* p_parent, float p_cost)
{
	/* Set the position equal to the parameter position */
	pos = p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Initialize the parent hash table */
	parents = std::unordered_map<unsigned int, Coord>();
	parents_ptr = &parents;
	parents_copied = true;

	/* Add the parent node */
	if (p_parent != NULL)
	{
		Coord parent_coord = *p_parent->get_pos()->get_coord();
		parents.emplace(HashStruct::hash_coord(&parent_coord),parent_coord);
	}
	
	/* Not marked for deletion by default */
	del_mark = false;
}

/*
* Constructor to create an AStarNode from another AStarNode
* @param a_star_node: The A* Node on which this A* Node will be based
*/
AStarNode::AStarNode(AStarNode* a_star_node)
{
	/* Set the position equal to the parameter A* Node's position */
	pos = *(a_star_node->get_pos());

	/* Set the cost of the node */
	cost = a_star_node->get_cost();

	/* Get parameter A* Node's parents */
	parents = std::unordered_map<unsigned int, Coord>();
	parents_ptr = a_star_node->get_parents();
	parents_copied = false;

	/* Not marked for deletion by default */
	del_mark = a_star_node->del_mark;
}

/*
* Get the first parent coordinate of this position
* @return the first parent Coord in the hash table of parents
*/
Coord* AStarNode::get_parent()
{ 
	if (parents_ptr->size() == 0)
		return NULL;
	return &parents_ptr->begin()->second;
}

/*
* Add a parent if it is not already in the parents table 
* @param parent: The parent node to add
*/
void AStarNode::add_parent(AStarNode* parent)
{
	/* Parent list being altered, copy-on-write */
	if (parents_copied == false)
	{
		copy_parents();
		parents_copied = true;
	}

	/* Get the Coord of the parent */
	Coord parent_coord = parent->get_pos()->get_coord();

	/* Check if the parent is in the table */
	auto it = parents.find(HashStruct::hash_coord(&parent_coord));

	/* Place the parent in the hash table if it is not already there */
	if (it == parents.end())
	{
		parents.emplace(HashStruct::hash_coord(&parent_coord), parent_coord);
		return;
	}

	/* Parent was already in the table, an error must have occurred */
	throw TerminalException("Added pre-existing parent to parent list of an A* Node");
}

/*
* Remove a parent node from the list of parents
* @param parent: The coordinate of the parent node to remove
*/
void AStarNode::del_parent(Coord* parent)
{
	/* Parent list being altered, copy-on-write */
	if (parents_copied == false)
	{
		copy_parents();
		parents_copied = true;
	}

	/* Find the parent iterator */
	auto parent_it = parents.find(HashStruct::hash_coord(parent));

	/* Make sure the node exists */
	if (parent_it == parents.end())
		throw TerminalException("Tried to decrement a non-existant parent node.");

	/* Remove the parent from the list */
	parents.erase(parent_it);
}

/*
* Comparison operator for the AStarNode
*/
bool AStarNode::operator<(const AStarNode& comp) const
{
	/* First compare by cost*/
	if (cost > comp.cost)
		return false;
	else if (cost < comp.cost)
		return true;

	/* Compare by x coordinate */
	if (pos.get_x_coord() > comp.pos.get_x_coord())
		return false;
	else if (pos.get_x_coord() < comp.pos.get_x_coord())
		return true;

	/* Compare by y coordinate */
	if (pos.get_y_coord() > comp.pos.get_y_coord())
		return false;
	else if (pos.get_y_coord() < comp.pos.get_y_coord())
		return true;

	/* Compare by depth */
	if (pos.get_depth() > comp.pos.get_depth())
		return false;
	else if (pos.get_depth() < comp.pos.get_depth())
		return true;

	/* If all of the above are equal, the nodes are the same */
	throw TerminalException("Equal nodes compared in priority queue.");
}

/*
* Comparison operator for the AStarNode
*/
bool AStarNode::operator>(const AStarNode& comp) const
{
	/* First compare by cost*/
	if (cost > comp.cost)
		return true;
	else if (cost < comp.cost)
		return false;

	/* Compare by x coordinate */
	if (pos.get_x_coord() > comp.pos.get_x_coord())
		return true;
	else if (pos.get_x_coord() < comp.pos.get_x_coord())
		return false;

	/* Compare by y coordinate */
	if (pos.get_y_coord() > comp.pos.get_y_coord())
		return true;
	else if (pos.get_y_coord() < comp.pos.get_y_coord())
		return false;

	/* Compare by depth */
	if (pos.get_depth() > comp.pos.get_depth())
		return true;
	else if (pos.get_depth() < comp.pos.get_depth())
		return false;

	return false;
}

/* Print parents */
void AStarNode::print_parents()
{
	std::cout << "PARENTS: " << std::endl;
	for (auto it = parents_ptr->begin(); it != parents_ptr->end(); it++)
		std::cout << it->second << std::endl;
}

/*
* Copy the parents list if it is written to
*/
void AStarNode::copy_parents()
{
	/* Copy list of parents to this list of parents and update pointer */
	parents = *parents_ptr;
	parents_ptr = &parents;

	/* Update variable to reflect that copy occurred */
	parents_copied = true;
}