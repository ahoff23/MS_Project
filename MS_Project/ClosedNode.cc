#include "ClosedNode.h"
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
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param parent: Parent of this node, nullptr if no parent exists
*/
ClosedNode::ClosedNode(Position& p_pos, ClosedNode* parent)
{
	/* Set the position equal to the parameter position */
	pos = Position();

	/* Create an empty hash table for the parents */
	parents = std::unordered_map<int, ClosedNode*>();

	if (parent == nullptr)
		return;

	/* Add the parent node */
	int parent_hash = CantorPair::get_int(&(parent->get_pos()));
	parents.emplace(parent_hash, parent);
}

