#include "AStarNode.h"
#include "CantorPair.h"

/*
* Default constructor
*/
AStarNode::AStarNode()
{
	/* Set the position as the default position */
	pos = Position();

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, AStarNode*>();

	/* 
	* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;

	/* Initialize the parent count */
	parent_count = 0;
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param parent: Parent of this node, nullptr if no parent exists
* @param p_cost: The cost of the node
*/
AStarNode::AStarNode(Position* p_pos, AStarNode* parent, float p_cost)
{
	/* Set the position equal to the parameter position */
	pos = *p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, AStarNode*>();

	/* Initialize the parent count */
	parent_count = 0;

	/* Make sure the parent pointer exists */
	if (parent == nullptr)
		return;

	/* Add the parent node */
	int parent_hash = CantorPair::get_int(parent->get_pos());
	parents.emplace(parent_hash, parent);
	parent_count++;
}

/*
* Constructor to create a AStarNode from another AStarNode
* @param open_node: The OpenNode on which this AStarNode will be based
*/
AStarNode::AStarNode(AStarNode* a_star_node)
{
	/* Set the position equal to the OpenNode's position */
	pos = *(a_star_node->get_pos());

	/* Set the cost of the node */
	cost = a_star_node->get_cost();

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, AStarNode*>();

	/* Get open_node's parent */
	AStarNode* parent = a_star_node->get_parent();

	/* Initialize the parent count */
	parent_count = 0;

	/* Make sure open_node had a parent */
	if (parent == nullptr)
		return;

	/* Add the parent node */
	int parent_hash = CantorPair::get_int(parent->get_pos());
	parents.emplace(parent_hash, parent);
	parent_count++;
}

/* 
* Add a parent to the hash table of parents if it is not already there
* @param new_parent: Pointer to the parent to add to the hash table of parents
*/
void AStarNode::add_parent(AStarNode* new_parent)
{
	/* Get the cantor pair hash to the new parent */
	int hash = CantorPair::get_int(new_parent->get_pos());

	/* Add the parent to the hash table if it is not already there */
	if (parents.find(hash) != parents.end())
	{
		parents.emplace(hash, new_parent);

		/* Increment the number of parents pointing at this node */
		parent_count++;
	}
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