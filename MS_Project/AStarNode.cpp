#include "AStarNode.h"
#include "Exceptions.h"
#include "HashStruct.h"


int AStarNode::pub_count = 0;
#include <iostream>


/*
* Default constructor
*/
AStarNode::AStarNode()
{

	count = pub_count;
	pub_count++;












	/* Set the position as the default position */
	Position* temp = new Position();
	pos = *temp;
	delete temp;

	/* No parent for this node */
	parents = std::unordered_map<unsigned int, Coord>();

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

	count = pub_count;
	pub_count++;








	/* Set the position equal to the parameter position */
	pos = p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Initialize the parent hash table */
	parents = std::unordered_map<unsigned int, Coord>();

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

	count = pub_count;
	pub_count++;









	/* Set the position equal to the parameter A* Node's position */
	pos = *(a_star_node->get_pos());

	/* Set the cost of the node */
	cost = a_star_node->get_cost();

	/* Get parameter A* Node's parents */
	parents = *a_star_node->get_parents();

	/* Not marked for deletion by default */
	del_mark = false;
}

/*
* Get the first parent coordinate of this position
* @return the first parent Coord in the hash table of parents
*/
Coord* AStarNode::get_parent()
{ 
	if (parents.size() == 0)
		return NULL;
	return &parents.begin()->second;
}

/*
* Add a parent if it is not already in the parents table 
* @param parent: The parent node to add
*/
void AStarNode::add_parent(AStarNode* parent)
{
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
	return cost < comp.cost;
}

/*
* Comparison operator for the AStarNode
*/
bool AStarNode::operator>(const AStarNode& comp) const
{
	return cost > comp.cost;
}

/* Print parents */
void AStarNode::print_parents()
{
	std::cout << "PARENTS: " << std::endl;
	for (auto it = parents.begin(); it != parents.end(); it++)
		std::cout << it->second << std::endl;
}