#include "AStarNode.h"
#include "CantorPair.h"

/*
* Default constructor
*/
AStarNode::AStarNode()
{
	/* Set the position as the default position */
	pos = Position();

	/* No parent for this node */
	parent = nullptr;

	/* 
	* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;

	/* Without a parent no path leads to this node */
	point_count = 0;
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param p_parent: Parent of this node, nullptr if no parent exists
* @param p_cost: The cost of the node
*/
AStarNode::AStarNode(Position p_pos, AStarNode* p_parent, float p_cost)
{
	/* Set the position equal to the parameter position */
	pos = p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Make sure the parent pointer exists */
	if (p_parent == nullptr)
	{
		point_count = 0;
		return;
	}

	/* Add the parent node */
	parent = p_parent;

	/* The parent path leads to this node */
	point_count = 1;
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

	/* Get open_node's parent */
	parent = a_star_node->get_parent();
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