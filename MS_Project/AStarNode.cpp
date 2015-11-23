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
	parents = 0;

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
AStarNode::AStarNode(Position* p_pos, AStarNode* p_parent, double p_cost)
{
	/* Set the position equal to the parameter position */
	pos = *p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Add the parent node */
	if (p_parent != NULL)
	{
		parents =
			HashStruct::hash_coord_comp(p_parent->get_pos()->get_coord(), p_pos->get_coord());
	}
	else
		parents = 0;
	
	/* Not marked for deletion by default */
	del_mark = false;
}

/*
* Constructor to create an AStarNode from another AStarNode
* @param a_star_node: The A* Node on which this A* Node will be based
*/
#include <bitset>
AStarNode::AStarNode(AStarNode* a_star_node)
{
	/* Set the position equal to the parameter A* Node's position */
	pos = *(a_star_node->get_pos());

	/* Set the cost of the node */
	cost = a_star_node->get_cost();

	/* Get parameter A* Node's parents */
	parents = a_star_node->get_parents();

	/* Not marked for deletion by default */
	del_mark = a_star_node->del_mark;
}

/*
* Get the first parent coordinate of this position
* @return the first parent Coord in the hash table of parents
*/
Coord AStarNode::get_parent()
{ 
	/* Throw an error if no parent is found. */
	if (parents == 0)
		throw TerminalException("No parent found for a node.");

	/* Find the first bit that is a 1 in the parents bitmap */
	unsigned short comp = 1;
	while ((comp & parents) == 0)
		comp = comp << 1;

	/* Convert that bit back to a coordinate */
	return HashStruct::hash_to_coord(comp,pos.get_coord());
}

/*
* Add a parent if it is not already in the parents table 
* @param parent: The parent node to add
*/
void AStarNode::add_parent(AStarNode* parent)
{
	/* Get the hash of the coordinate */
	unsigned short hash = 
		HashStruct::hash_coord_comp(parent->get_pos()->get_coord(), pos.get_coord());

	/* Make sure the parent is not already in the bitmap */
	if ((parents & hash) != 0)
		throw TerminalException("Added pre-existing parent to parent list of an A* Node");

	/* Update the parents bitmap */
	parents |= hash;
}

/*
* Remove a parent node from the list of parents
* @param parent: The coordinate of the parent node to remove
* @return the parent bitmap
*/
unsigned short AStarNode::del_parent(Coord* parent)
{
	/* Get the hash of the coordinate */
	unsigned short hash =
		HashStruct::hash_coord_comp(parent, pos.get_coord());

	/* Make sure the node exists */
	if ((hash & parents) == 0)
		throw TerminalException("Tried to decrement a non-existant parent node.");

	/* Remove the parent from the list */
	parents &= ~hash;

	return parents;
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

/*
* Print parents 
*/
void AStarNode::print_parents()
{
	/* Number of possible parenst */
	const unsigned short NUM_PARENTS = 9;

	/* Value to get the correct bit from the parents bitmap */
	int bit_trav = 1;

	for (int i = 0; i < NUM_PARENTS; i++)
	{
		Coord print_coord = HashStruct::hash_to_coord(bit_trav, pos.get_coord());
		if ((parents & bit_trav) != 0)
			std::cout << print_coord << std::endl;
		bit_trav = bit_trav << 1;
	}
}

/* 
* Check if a parent is in the list of parents 
* @param parent: The parent to check for
* @return true if the parent is found, false otherwise
*/
bool AStarNode::check_parent(Coord* parent)
{
	/* Get the hash for the parent */
	unsigned short hash = HashStruct::hash_coord_comp(parent, pos.get_coord());

	/* No parents in bitmap */
	if (parent == NULL)
	{
		if (hash != 0)
			return false;
		return true;
	}

	/* Compare the hash to the list of parents */
	if ((hash & parents) == 0)
		return false;
	return true;
}