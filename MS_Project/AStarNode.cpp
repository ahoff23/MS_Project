#include "AStarNode.h"
#include "CantorPair.h"

/*
* Default constructor
*/
AStarNode::AStarNode()
{
	/* Set the position as the default position */
	Position* temp = new Position();
	pos = *temp;
	delete temp;

	/* No parent for this node */
	parents = std::unordered_map<int,std::pair<AStarNode*,int> >();

	/* 
	* Default cost is -1 so the user can confirm this node has
	* not been initalized
	*/
	cost = -1;
}

/*
* Constructor with position and one parent
* @param p_pos: Position of the node
* @param p_parent: Parent of this node, NULL if no parent exists
* @param p_cost: The cost of the node
*/
AStarNode::AStarNode(Position p_pos, AStarNode* p_parent, float p_cost)
{
	/* Set the position equal to the parameter position */
	pos = p_pos;

	/* Set the cost of the node */
	cost = p_cost;

	/* Initialize the parent hash table */
	parents = std::unordered_map<int, std::pair<AStarNode*, int> >();
	
	/* Add the parent node */
	int hash = CantorPair::get_int(p_parent);
	std::pair<AStarNode*, int> pair = std::pair<AStarNode*, int>(p_parent, 1);
	parents.emplace(hash,pair);
}

/*
* Constructor to create an AStarNode from another AStarNode
* @param open_node: The OpenNode on which this AStarNode will be based
*/
AStarNode::AStarNode(AStarNode* a_star_node)
{
	/* Set the position equal to the OpenNode's position */
	pos = *(a_star_node->get_pos());

	/* Set the cost of the node */
	cost = a_star_node->get_cost();

	/* Get open_node's parent */
	parents = *a_star_node->get_parents();
}

/*
* Get the first A* Node position of this node
* @return the first parent A* Node in the hash table of parents
*/
AStarNode * AStarNode::get_parent()
{ 
	if (parents.size() == 0)
		return NULL;

	return parents.begin()->second.first; 
}

/*
* Add a parent if it is not already in the parents table 
* @return true if a new parent is added, false if it already is in the table
*/
bool AStarNode::add_parent(AStarNode* parent)
{
	/* Get the hash of the parent */
	int hash = CantorPair::get_int(parent);

	/* Check if the parent is in the table */
	auto it = parents.find(hash);

	/* Parent is not in the table */
	if (it == parents.end())
	{
		/*
		* Create a pair for the parent 
		* The first element is a pointer to the parent A* Node and
		* the second is a counter of how many paths from the parent
		* lead to this position
		*/
		std::pair<AStarNode*, int> pair = std::pair<AStarNode*, int>(parent, 1);

		/* Place the parent in the hash table */
		parents.emplace(hash, pair);
		return true;
	}

	/* Parent was already in the table, increment its counter */
	it->second.second += 1;
	return false;
}

/*
* Remove a parent node (decrement the counter) 
* @param parent: The parent node to remove
* @return true if the node has any parents left, false if it has none
*/
bool AStarNode::dec_parent(AStarNode* parent)
{
	/* Get the hash value of the parent */
	int hash = CantorPair::get_int(parent);

	/* Search for the parent in the parents hash table */
	auto it = parents.find(hash);

	/* If the parent is found, decrement its counter by 1 */
	if (it != parents.end())
	{
		it->second.second -= 1;

		/* If the counter is decreased to 0, remove the parent */
		if (it->second.second == 0)
			parents.erase(hash);

		/* If there are no parents left, return false */
		if (parents.size() == 0)
			return false;
	}
	return true;
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