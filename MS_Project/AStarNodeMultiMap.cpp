#include <iostream>

#include "AStarNodeMultiMap.h"
#include "AStarNodeList.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "HashStruct.h"

/*
* Constructor simply initializes the map 
*/
AStarNodeMultiMap::AStarNodeMultiMap()
{
	map = std::unordered_multimap<unsigned int, AStarNode*>();
}

/*
* Place a new A* Node in the map 
* @param node: The node to place in the map
*/
void AStarNodeMultiMap::emplace(AStarNode* node)
{
	map.emplace(HashStruct::hash_pos(node->get_pos()), node);
}

/* 
* Find an object in the map based on a key
* @param pos: The key to find the AStarNode in the map
*/
AStarNode* AStarNodeMultiMap::find(Position* pos)
{
	/* Get the iterator for the position */
	auto it = find_it(pos);

	/* If the iterator cannot be found return NULL */
	if (it == map.end())
		return NULL;

	/* Return the object corresponding to key */
	return it->second;
}

/*
* Erase an A* node from the map
* @param pos: The key of the object to erase
* @return true if the node is erased, false if it cannot be found
*/
bool AStarNodeMultiMap::erase(Position* pos)
{
	/* Get the iterator for the position */
	std::unordered_multimap<unsigned int, AStarNode*>::iterator it = find_it(pos);

	/* If the iterator cannot be found do nothing */
	if (it == map.end())
		return false;

	/* Erase the iterator */
	map.erase(it);
	return true;
}

/* 
* Get an iterator based on a position 
* @param pos: The position to use as a key
* @return an iterator if a corresponding position is found, or the end of the map if not
*/
std::unordered_multimap<unsigned int, AStarNode*>::iterator
	AStarNodeMultiMap::find_it(Position* pos)
{
	/* Find the first iterator pointing to a key of pos */
	std::unordered_multimap<unsigned int, AStarNode*>::iterator it =
		map.find(HashStruct::hash_pos(pos));

	/* If no node contains this key, return a NULL pointer */
	if (it == map.end())
		return map.end();

	/* Store the key of the iterator */
	unsigned int hash_val = HashStruct::hash_pos(pos);

	/* Check all elements with this key */
	while (it->first == hash_val)
	{
		/* Compare coordinates and depth */
		if (*pos == *it->second->get_pos())
			return it;

		/* Move to the next element */
		it++;
	}

	/* The exact node could not be found */
	return map.end();
}

/* 
* Copy this map's contents into another map
* @param copy_map: The map to copy the map's contents into
*/
void AStarNodeMultiMap::node_copy(AStarNodeMultiMap* copy_map)
{
	/* Copy and place each node into this list */
	for (auto it = map.begin(); it != map.end(); ++it)
	{
		AStarNode* new_node = new AStarNode(it->second);
		copy_map->emplace(new_node);
	}
}

/* Place all elements in the map into a heap */
void AStarNodeMultiMap::heap_place(
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >* heap
	)
{
	/* Iterate through each element in the hash table */
	for (auto it = map.begin(); it != map.end(); it++)
		heap->emplace(it->second);
}

/*
* Print out all elements in the map 
*/
void AStarNodeMultiMap::print_map()
{
	std::cout << "LIST CONTENTS: " << std::endl;
	for (auto it = map.begin(); it != map.end(); it++)
		std::cout << it->second->get_pos() << std::endl;
}

/* 
* Destructor 
*/
AStarNodeMultiMap::~AStarNodeMultiMap()
{
	/* Delete all nodes in the list */
	for (auto it = map.begin(); it != map.end(); ++it)
		delete it->second;
}