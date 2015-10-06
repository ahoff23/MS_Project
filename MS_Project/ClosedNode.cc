#include "ClosedNode.h"
#include "OpenNode.h"
#include "CantorPair.h"

/*
* Default constructor
*/
ClosedNode::ClosedNode()
{
	/* Set the position as the default position */
	pos = Position();

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, ClosedNode*>();

	/* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param parent: Parent of this node, nullptr if no parent exists
* @param p_cost: The cost of the node
*/
ClosedNode::ClosedNode(Position& p_pos, ClosedNode* parent, int p_cost)
{
	/* Set the position equal to the parameter position */
	pos = p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, ClosedNode*>();

	/* Make sure the parent pointer exists */
	if (parent == nullptr)
		return;

	/* Add the parent node */
	int parent_hash = CantorPair::get_int(parent->get_pos());
	parents.emplace(parent_hash, parent);
}

/*
* Constructor to create a ClosedNode from an OpenNode
* @param open_node: The OpenNode on which this ClosedNode will be based
*/
ClosedNode::ClosedNode(OpenNode* open_node)
{
	/* Set the position equal to the OpenNode's position */
	pos = open_node->get_pos();

	/* Set the cost of the node */
	cost = open_node->get_cost();

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, ClosedNode*>();

	/* Get open_node's parent */
	ClosedNode* parent = open_node->get_parent();

	/* Make sure open_node had a parent */
	if (parent == nullptr)
		return;

	/* Add the parent node */
	int parent_hash = CantorPair::get_int(parent->get_pos());
	parents.emplace(parent_hash, parent);
}