#include "OpenNode.h"
#include "ClosedNode.h"
#include "CantorPair.h"

/*
* Default Constructor
*/
OpenNode::OpenNode()
{
	/* Default OpenNode has no parent */
	parent = nullptr;

	/* Default position */
	pos = Position();

	/* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;
}

/*
* Constructor with parameters
* @param p_parent: Pointer to the ClosedNode containing the parent of this node
* @param p_pos: The position of this node
* @param p_cost: The cost of the node
*/
OpenNode::OpenNode(ClosedNode* p_parent, Position& p_pos, int p_cost)
{
	parent = p_parent;
	pos = p_pos;
	p_cost = cost;
}

/*
* Comparison operator for the OpenNode to be used in a MinHeap
*/
bool OpenNode::operator<(const OpenNode & comp) const
{
	return cost < comp.cost;
}