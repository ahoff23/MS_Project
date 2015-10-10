#include "AStarNodePointer.h"
#include "AStarNode.h"

/* 
* Constructor
* @param p_ptr: A* Node to point to
*/
AStarNodePointer::AStarNodePointer(AStarNode* p_ptr)
{
	ptr = p_ptr;
	counter = 0;
}

/* 
* Destructor 
*/
AStarNodePointer::~AStarNodePointer()
{
	delete ptr;
}