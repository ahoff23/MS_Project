#ifndef PATHCLEARASTAR_H
#define PATHCLEARASTAR_H

#include <queue>
#include <functional>
#include <unordered_map>

class Agent;
class AStarNodeList;
class AStarNode;
class Position;
class World;
class Coord;

class PathClearAStar
{
public:
	/* 
	* Get the goal coordinate from the parent search as well as pointers
	* to its OPEN and CLOSED hash tables to be able to remove nodes in those
	* hash tables if they are also found in this A* Search
	*/
	PathClearAStar(Agent* search, Position* start_pos);

	/* Clear the OPEN and CLOSED lists of descendants of start_pos */
	void path_clear_a_star();

	/* Destructor */
	~PathClearAStar();
private:
	/* Pointer to the world of agents */
	World* world;
	/* Goal coordinate of the agent */
	Coord* goal;
	/* OPEN list in the form of a min heap */
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> > open_list;
	/* OPEN list in the form of a hash table */
	AStarNodeList* open_list_hash_table;
	/* CLOSED list for the search in the form of a hash table */
	AStarNodeList* closed_list;
	/*
	* Hash table for the conflicts at this node. The key is the cantor pair of the Position
	* and the value is simply a meaningless boolean value. The value is meaningless because
	* the only thing that matters is whether or not the Position is in the hash table
	*/
	std::unordered_map<unsigned int, Position>* constraints;
	/* Pointer to the OPEN hash table of the parent A* Search */
	AStarNodeList* parent_open_list;
	/* Pointer to the CLOSED hash table of the parent A* Search */
	AStarNodeList* parent_closed_list;
	/* Calculate the cost of a position */
	float calc_cost(Position* pos);
	/* Get a vector of successor positions of a given position */
	void get_successors(Position* pos, std::vector<Position>* successors);
};

#endif