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
}

/*
* Constructor with parameters
* @param p_parent: Pointer to the ClosedNode containing the parent of this node
* @param p_pos: The position of this node
*/
OpenNode::OpenNode(ClosedNode& p_parent, Position& p_pos)
{
	parent = &p_parent;
	pos = p_pos;
}