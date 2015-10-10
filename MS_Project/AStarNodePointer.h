#ifndef ASTARNODEPOINTER_H
#define ASTARNODEPOINTER_H

class AStarNode;

class AStarNodePointer
{
public:
	/* Constructors */
	AStarNodePointer(AStarNode* p_ptr);
	
	/* Increment and decrement the counter*/
	void inc() { counter++; };
	void dec() { counter--; };

	/* Accessors */
	int get_counter() { return counter; };

	/* Destructor */
	~AStarNodePointer();
private:
	/* Pointer to an A* Node */
	AStarNode* ptr;

	/* Counter of how many paths led to the ptr node */
	int counter;
};

#endif