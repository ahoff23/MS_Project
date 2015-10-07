#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>
#include <stack>

class Coord;
class AStarNode;
class ClosedList;
class Position;
class World;

/*
* Class for performing an A* search based on the world and an
* agent's start and goal coordinates
*/
class AStar
{
public:
	/* Initialize the A* search and place the root on the OPEN list */
	AStar(Coord* p_start, Coord* p_goal, World* p_world);

	/* Initialize the A* search from a pre-existing A* search */
	AStar(
		Coord* p_start, Coord* p_goal, 
		std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>* p_open_list,
		std::unordered_multimap<int, AStarNode*>* p_open_list_hash_table,
		std::unordered_map<int, bool>* p_constraints,
		World* p_world
		);

	/* Add a conflict to the conflict hash table */
	void add_conflict(Position* conflict);

	/* Perform the A* search and save the goal node as goal_node */
	void find_solution();

	/* Create a Flag A* search object to mark the OpenNodes to not be expanded */
	void clear_frontier();

	/* Return the solution as a stack of coordinates*/
	std::stack<Coord> get_solution();

	/* Destructor */
	~AStar();
private:
	/* Pointer to the world of agents */
	World* world;

	/* Start coordinate of the agent */
	Coord* start;

	/* Goal coordinate of the agent */
	Coord* goal;

	/* Goal node of the search */
	AStarNode* goal_node;

	/* OPEN list in the form of a min heap */
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>> open_list;

	/* 
	* OPEN list in the form of a hash table where the hash is the cantor pair of the Position
	* object of the OpenNode and the value is all positions preceding this position.
	*/
	std::unordered_multimap<int, AStarNode*> open_list_hash_table;
	
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

	/* Get a vector of successor positions of a given position */
	void get_successors(Position* pos, std::vector<Position>* successors);

	/* Check if a node is in the OPEN list */
	int check_duplicate_open(Position* pos, Position* parent);
};

#endif