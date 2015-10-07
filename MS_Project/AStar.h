#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>

class Coord;
class AStarNode;
class ClosedList;
class Position;

/*
* Class for performing an A* search based on the world and an
* agent's start and goal coordinates
*/
class AStar
{
public:
	/* Initialize the A* search and place the root on the OPEN list */
	AStar(Coord* p_start, Coord* p_goal);

	/* Initialize the A* search from a pre-existing A* search */
	AStar(
		Coord* p_start, Coord* p_goal, 
		std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>* p_open_list,
		std::unordered_map<int, AStarNode*>* p_open_list_hash_table,
		std::unordered_map<int, bool>* p_constraints
		);

	/* Add a conflict to the conflict hash table */
	void add_conflict(Position* conflict);

	/* 
	* Perform the A* search and return the goal node. This function
	* places the responsibility of de-allocating the dynamic memory of the
	* return node on the function caller. 
	*/
	AStarNode * find_solution();

	/* Create a Flag A* search object to mark the OpenNodes to not be expanded */
	void clear_frontier();

	/* Destructor */
	~AStar();
private:
	/* Start coordinate of the agent */
	Coord* start;

	/* Goal coordinate of the agent */
	Coord* goal;

	/* OPEN list in the form of a min heap */
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>> open_list;

	/* 
	* OPEN list in the form of a hash table where the hash is the cantor pair of the Position
	* object of the OpenNode and the value is the number of paths leading to the OpenNode.
	*/
	std::unordered_map<int, AStarNode*> open_list_hash_table;
	
	/* CLOSED list for the search */
	ClosedList* closed_list;

	/*
	* Hash table for the conflicts at this node. The key is the cantor pair of the Position
	* and the value is simply a meaningless boolean value. The value is meaningless because
	* the only thing that matters is whether or not the Position is in the hash table
	*/
	std::unordered_map<int, bool> constraints;

	/* Place a node in the OPEN list */
	void add_open(Position* pos, AStarNode* parent);

	/* Calculate the cost of a position */
	float calc_cost(Position* pos);
};

#endif