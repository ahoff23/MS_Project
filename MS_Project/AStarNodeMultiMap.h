#ifndef ASTARNODEMULTIMAP_H
#define ASTARNODEMULTIMAP_H

#include <unordered_map>
#include <queue>
#include <functional>

class Position;
class AStarNode;

/* Class containing an unordered multimap with A* Node's as mapped values  */
class AStarNodeMultiMap
{
public:
	AStarNodeMultiMap();
	/* Place a new A* Node in the map */
	void emplace(AStarNode* node);
	/* Find an element in the map based on a key */
	AStarNode* find(Position* pos);
	/* Erase an object from the map */
	bool erase(Position* pos);
	/* Clear the map */
	void clear() { map.clear(); };
	/* Copy the map's contents into another map */
	void node_copy(AStarNodeMultiMap* copy_map);
	/* Place all elements in the map into a heap */
	void heap_place(
		std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >* heap
		);
	/* Print out all elements in the map */
	void print_map();
	
	/* Accessors */
	unsigned int size() const {	return map.size(); };

	/* Destructor */
	~AStarNodeMultiMap();
private:
	/* Map for A* Nodes */
	std::unordered_multimap<unsigned int, AStarNode*> map;

	/* Get an iterator based on a position */
	std::unordered_multimap<unsigned int, AStarNode*>::iterator
		find_it(Position* pos);
};

#endif