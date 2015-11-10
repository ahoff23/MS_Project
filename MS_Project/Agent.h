#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include <stack>

#include "Macros.h"
#include "Coordinates.h"

#ifdef TIME_LIMIT
#include <ctime>
#endif

class AStarNode; 
class AStarNodeList;
class World;
class PathClearAStar;

/*
* Class for performing an A* search based on the world and an
* agent's start and goal coordinates
*/
class Agent
{
public:

	/* Initialize the A* search and place the root on the OPEN list */
#ifdef TIME_LIMIT
	Agent(
		Coord* p_start, Coord* p_goal, World* p_world,
		std::string p_name, std::time_t p_start_time
		);
#else
	Agent(Coord* p_start, Coord* p_goal, World* p_world, std::string p_name);
#endif
	/* Initialize the A* search from a pre-existing agent */
	Agent(Agent* p_agent, Position* new_constraint);

	/* Add a conflict to the conflict hash table */
	void add_conflict(Position* conflict);
	/* Perform the A* search and save the goal node as goal_node */
	void find_solution();
	/* Return the solution as a stack of coordinates*/
	std::stack<Coord> get_solution();
	/* Print the solution to the console */
	void print_solution();
	/* Print the solution to a file */
	void file_print_solution(std::ofstream& file);
	/* Get the agent's cost */
	int get_cost();

	/* Accessor functions */
	Coord* get_goal() { return goal; };
	Coord* get_start() { return start_coord; };
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >*
		get_open_list() { return &open_list; };
	AStarNodeList* get_open_list_hash_table() { return open_list_hash_table; };
	AStarNodeList* get_closed_list() { return closed_list; };
	std::unordered_map<unsigned int, Position>* get_constraints() { return &constraints; };
	World* get_world() { return world; };
	std::string get_name() { return name; };

#ifdef OPEN_LIST_DATA
	/* agent_depth accessor function */
	unsigned short get_depth() { return agent_depth; };
#endif

	/* Destructor */
	~Agent();
private:
	/* Pointer to the world of agents */
	World* world;
	/* Goal coordinate of the agent */
	Coord* goal;
	/* Start coordinate of the agent */
	Coord* start_coord;
	/* Goal node of the search */
	AStarNode* goal_node;
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
	std::unordered_map<unsigned int, Position> constraints;
	/*
	* Sub-search to find nodes to remove from the OPEN and CLOSED list
	* based on a new constraint.
	*/
	PathClearAStar* path_clear;
	/* Calculate the cost of a position */
	float calc_cost(Position* pos);
	/* Name of the agent */
	std::string name;

	/* Get a vector of successor positions of a given position */
	void get_successors(Position* pos, std::vector<Position>* successors);

#ifdef OPEN_LIST_DATA
	/* Agent's depth  (i.e. number of ancestor agents) */
	int agent_depth;
#endif
#ifdef TIME_LIMIT
	std::time_t start_time;
#endif
};

#endif