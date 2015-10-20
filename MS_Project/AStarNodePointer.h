#ifndef ASTARNODEPOINTER_H
#define ASTARNODEPOINTER_H

class AStarNode;

class AStarNodePointer
{
public:
	/* Constructors */
	AStarNodePointer(AStarNode* p_ptr);

	/* Manually set the counter of the pointer */
	void set_counter(int p_counter);
	
	/* Increment and decrement the counter*/
	void inc() { counter++; };
	void dec() { counter--; };

	/* Accessors */
	int get_counter() { return counter; };
	AStarNode* get_ptr() { return ptr; };

	/* Destructor */
	~AStarNodePointer();
private:
	/* Pointer to an A* Node */
	AStarNode* ptr;
	/* Counter of how many paths led to the ptr node */
	int counter;
};

#endif