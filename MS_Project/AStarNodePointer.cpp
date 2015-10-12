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
* Manually set the counter of the pointer 
* @param p_counter: The value to set the counter to
*/
void AStarNodePointer::set_counter(int p_counter)
{
	counter = p_counter;
}

/* 
* Destructor 
*/
AStarNodePointer::~AStarNodePointer()
{
	delete ptr;
}