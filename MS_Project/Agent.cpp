#include <math.h>
#include <iostream>
#include <fstream>

#include "Agent.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "World.h"
#include "AStarNodeList.h"
#include "Exceptions.h"
#include "HashStruct.h"

#ifndef CBS_CLASSIC
#include "PathClearAStar.h"
#endif

/* 
* Initialize the agent search and place the root on the OPEN list 
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_world: Pointer to the world this search will navigate
* @param p_name: The agent's name
* @param start_time: The start time of the CBS Tree search used 
* for early termination time limits
*/
#ifdef TIME_LIMIT
Agent::Agent(
	Coord* p_start, Coord* p_goal, World* p_world,
	std::string p_name, std::time_t p_start_time
	)
#else
Agent::Agent(Coord* p_start, Coord* p_goal, World* p_world, std::string p_name)
#endif
{
	/* Get the goal coordinate of the A* Search */
	goal = new Coord(p_goal);

	/* No goal node is found initially */
	goal_node = NULL;

	/* Store the start coord */
	start_coord = new Coord(p_start);

	/* Initialize lists and hash tables */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();
	open_list_hash_table = new AStarNodeList();
	closed_list = new AStarNodeList();
	constraints = std::unordered_map<unsigned int, Position>();

	/* Place the root into the OPEN list */
	Position start_pos = Position(p_start, 0);
	AStarNode* start_node = new AStarNode(&start_pos, NULL, calc_cost(&start_pos));
	open_list.emplace(start_node);
	open_list_hash_table->add_node(start_node);

	/* Set the world to navigate */
	world = p_world;

	/* No new constraint */
	path_clear = NULL;

	/* Set the name of the agent */
	name = p_name;

#ifdef OPEN_LIST_DATA
	/* Track the depth of the agent */
	agent_depth = 0;
#endif

#ifdef TIME_LIMIT
	start_time = p_start_time;
#endif
}

/*
* Constructor that begins by eliminating nodes descended from constrained
* a position given as a parameter
* @param p_astar: The AStar list to copy
* @param new_constraint: The new constraint to remove all descendants of
* from the CLOSED and OPEN lists
*/
Agent::Agent(Agent* p_agent, Position* new_constraint)
{
	/* Get the start and goal coordinate of the A* Search */
	goal = new Coord(*(p_agent->get_goal()));

	/* No goal node is found initially */
	goal_node = NULL;

	/* Store the start Coord */
	start_coord = new Coord(p_agent->get_start());

	/* Set the world to navigate */
	world = p_agent->get_world();

	/* Set the name of the agent */
	name = p_agent->get_name();

	/* Initalize open and closed list hash tables */
	open_list_hash_table = new AStarNodeList();
	closed_list = new AStarNodeList();

	/* Copy the constraints and add the new constraint */
	constraints = *(p_agent->get_constraints());
	constraints.emplace(HashStruct::hash_pos(new_constraint), *new_constraint);

#ifdef CBS_CLASSIC
	/* Using CBS Classic (no PCA*) */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();

	/* Put the start node in the OPEN list */
	Position start_pos = Position(p_agent->get_start(), 0);
	AStarNode* start_node = new AStarNode(&start_pos, NULL, calc_cost(&start_pos));
	open_list.emplace(start_node);
	open_list_hash_table->add_node(start_node);
#else
	/* Copy OPEN list hash table */
	open_list_hash_table->node_copy(p_agent->get_open_list_hash_table());

	/* Place each node in the OPEN list hash table into the minheap */
	open_list = 
		std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();
	open_list_hash_table->heap_place(&open_list);

	/* Copy the closed list as well	*/
	closed_list->node_copy(p_agent->get_closed_list());

	/* Remove descendants of new_constraint from the OPEN and CLOSED lists */
	if (new_constraint != NULL)
	{
		path_clear = new PathClearAStar(this, new_constraint);
		path_clear->path_clear_a_star();
	}
#endif

#ifdef OPEN_LIST_DATA
	/* Increment the agents depth */
	agent_depth = p_agent->get_depth() + 1;
#endif

#ifdef TIME_LIMIT
	start_time = p_agent->start_time;
#endif
}

/*
* Add a conflict to the conflict hash table
* @param conflict: Pointer to the position to add to the list of conflicts
*/
void Agent::add_conflict(Position* conflict)
{
	constraints.emplace(HashStruct::hash_pos(conflict),*conflict);
}

/*
* Perform the A* search and save the goal node as goal_node
*/
void Agent::find_solution()
{
#ifdef OPEN_LIST_DATA
	/* Create a variable for the number of nodes popped from the OPEN list */
	int popped = 0;
	/* Create a variable for the number of nodes added to the OPEN list */
	int added = 0;
#endif
	
	while (!open_list.empty())
	{
#ifdef TIME_LIMIT
		/* Make sure the time limit has not been exceeded */
		if ((std::clock() - start_time) / CLOCKS_PER_SEC > TIME_LIMIT)
			throw TerminalException("TIME LIMIT EXCEEDED");
#endif
		/* Pop min cost from open_list and remove from hash table */
		AStarNode* heap_top = open_list.top();
		open_list.pop();

#ifndef CBS_CLASSIC
		/* Delete the node if it is marked for deletion */
		if (heap_top->get_del_mark() == true)
		{
			delete heap_top;
			continue;
		}
#endif

#ifdef A_STAR_SEARCH_DATA
		std::cout << "COORD: " <<  *heap_top->get_pos()->get_coord() <<
			" at depth " << heap_top->get_pos()->get_depth() << std::endl;
#endif

#ifdef OPEN_LIST_DATA
		/* Increment popped counter */
		popped++;
#endif

		/* Find the node on the OPEN list hash table */
		AStarNode* top;
		top = open_list_hash_table->check_duplicate(heap_top->get_pos());
		
		/* Make sure the node has not been removed from the list by PCA* */
		if (top == NULL || top != heap_top)
			throw TerminalException("Could not find node from heap in hash table.");

		/* Check if the node is a solution, if it is, return it */
		if (*top->get_pos()->get_coord() == *goal)
		{
			goal_node = top;

			/* Place the goal node back into the OPEN list (it will be removed by PCA*) */
			open_list.push(top);

#ifdef A_STAR_SEARCH_DATA
			std::cout << "END OF SEARCH" << std::endl;
#endif

#ifdef OPEN_LIST_DATA
			/* Print the OPEN list stats */
			std::cout << "AGENT DEPTH: " << agent_depth << std::endl;
			std::cout << "SIZE OF OPEN LIST: " << open_list.size() << std::endl;
			std::cout << "NUMBER OF POPPED NODES: " << popped << std::endl;
			std::cout << "SIZE OF OPEN LIST NODES IN THIS AGENT: " << added << std::endl;
#endif

#ifdef DISPLAY_LIST_SIZES
			/* Display the length of the OPEN and CLOSED lists */
			std::cout << "OPEN LIST SIZE: " << open_list_hash_table->get_list()->size() << std::endl;
			std::cout << "CLOSED LIST SIZE: " << closed_list->get_list()->size() << std::endl;
#endif
			return;
		}

		/* Generate successors */
		std::vector<Position> successors = std::vector<Position>();
		get_successors(top->get_pos(), &successors);

		/* For each successor, check if it is in the OPEN list and CLOSED list */
		int len = successors.size();
		for (int i = 0; i < len; i++)
		{
			/* Check if the successor is in either the OPEN or CLOSED list */
			AStarNode* check_open_list = 
				open_list_hash_table->check_duplicate(&successors[i]);
			AStarNode* check_closed_list = 
				closed_list->check_duplicate(&successors[i]);

			/* If the successor is not a duplicate, add it to the OPEN list */
			if (check_open_list == NULL && check_closed_list == NULL)
			{
				/* Create a new node and add it to the OPEN list (both heap and hash table) */
				AStarNode* add_node = 
					new AStarNode(&successors[i], top, calc_cost(&successors[i]));

				/* Add node to the hash table and minheap */
				open_list_hash_table->add_node(add_node);
				open_list.push(add_node);

#ifdef OPEN_LIST_DATA
				/* Increment added counter */
				added++;
#endif
			}
			else if (check_open_list != NULL)
				check_open_list->add_parent(top);
			else if (check_closed_list != NULL)
				check_closed_list->add_parent(top);
		}

		/* Add top to the CLOSED list */
		closed_list->add_node(top);

		/* Remove the node from the OPEN list without deleting the node */
		open_list_hash_table->remove_hash(top);
	}

	/* You should never run out of nodes in A* search */
	throw OutOfNodesException();
}

/* 
* Get a vector of successor positions of a given position 
* @param pos: The position whose successors will be found by this function
* @param successors: Vector which successors will be appended to
*/
void Agent::get_successors(Position* pos, std::vector<Position>* successors)
{
	/* Get the coordinates of pos */
	unsigned short x_coord = pos->get_coord()->get_xcoord();
	unsigned short y_coord = pos->get_coord()->get_ycoord();
	
	/* Get the new depth */
	unsigned short depth = pos->get_depth() + 1;
	
	/* Create positions for each possible successor */
	std::vector<Position*> possible_successors = std::vector<Position*>();
	possible_successors.push_back(new Position(x_coord + 1, y_coord, depth));
	possible_successors.push_back(new Position(x_coord + 1, y_coord + 1, depth));
	possible_successors.push_back(new Position(x_coord, y_coord, depth));
	possible_successors.push_back(new Position(x_coord, y_coord + 1, depth));

	/* X coordinate must be positive */
	if (x_coord > 0)
	{
		possible_successors.push_back(new Position(x_coord - 1, y_coord, depth));
		possible_successors.push_back(new Position(x_coord - 1, y_coord + 1, depth));

		/* Y coordinate must be positive */
		if (y_coord > 0)
			possible_successors.push_back(new Position(x_coord - 1, y_coord - 1, depth));
	}

	/* Y coordinate must be positive */
	if (y_coord > 0)
	{
		possible_successors.push_back(new Position(x_coord + 1, y_coord - 1, depth));
		possible_successors.push_back(new Position(x_coord, y_coord - 1, depth));
	}

	/* 
	* Add each possible successor if it is an existing coordinate, 
	* is not blocked by an obstacle and is not constrained.
	*/
	int num_successors = possible_successors.size();
	for (int i = 0; i < num_successors; i++)
	{
		if (
			world->check_coord(possible_successors[i]->get_coord()) &&
			constraints.find(HashStruct::hash_pos(possible_successors[i])) == constraints.end()
			)
			successors->push_back(*possible_successors[i]);
	}

	/* Delete all possible successors (pushed by to return list of successors by value) */
	for (int i = 0; i < num_successors; i++)
		delete possible_successors[i];
}

/*
* Return the solution as a stack of coordinates
* @return a stack of Coord objects, if find_solution() has not been
* called yet, call it.
*/
std::stack<Coord> Agent::get_solution()
{
	/* Initialize the stack */
	std::stack<Coord> path = std::stack<Coord>();

	/* Find the solution if it has not yet been found */
	if (goal_node == NULL)
		find_solution();

	/* Push the goal node's Coord object onto the stack */
	path.push(goal_node->get_pos()->get_coord());

	/* Get the depth of the goal node */
	unsigned short depth = goal_node->get_pos()->get_depth();

	/* Create a node pointer to traverse the nodes parents */
	Coord parent_coord = goal_node->get_parent();
	
	while (true)
	{
		/* Place the coordinate on the list */
		path.push(parent_coord);

		/* Update the cost of the parent coordinate (search backwards) */
		depth--;

		/* Construct a Position for the current coordinate */
		Position pos = Position(parent_coord, depth);

		/* Find the current node */
		AStarNode* curr_node = closed_list->check_duplicate(&pos);

		if (curr_node == NULL)
			throw TerminalException("Parent node in path search not found in CLOSED list.");

		/* Get the parent coordinate */
		if (curr_node->get_parent_bitmap() == 0)
			break;

		Coord temp_coord = curr_node->get_parent();
		parent_coord = temp_coord;
	}
	return path;
}

/*
* Print the solution to the console 
*/
void Agent::print_solution()
{
	/* Get the solution */
	std::stack<Coord> path = get_solution();

	/* Print the agent's name */
	std::cout << "*********************" << std::endl;
	std::cout << name << std::endl;
	std::cout << "*********************" << std::endl;

	/* Make sure the solution is correct */
	while (!path.empty())
	{
		std::cout << path.top() << std::endl;
		path.pop();
	}
}

/* 
* Print the solution to a file 
* @param file: The file stream to print to
*/
void Agent::file_print_solution(std::ofstream& file)
{
	/* Get the solution */
	std::stack<Coord> path = get_solution();

	/* Print the agent's name */
	file << "*********************" << "\r\n";
	file << name << "\r\n";
	file << "*********************" << "\r\n";

	/* Make sure the solution is correct */
	while (!path.empty())
	{
		file << path.top() << "\r\n";
		path.pop();
	}

	/* Add an extra line for formatting */
	file << "\r\n";
}

/*
* Calculate the cost of a position
* @param pos: The position to calculate the cost for
* @return the total cost of the position
*/
double Agent::calc_cost(Position* pos)
{
	/* Distances between pos and goal in X and Y axes */
	int x_diff = pos->get_coord()->get_xcoord() - goal->get_xcoord();
	int y_diff = pos->get_coord()->get_ycoord() - goal->get_ycoord();

	double heuristic = static_cast<double>(sqrt(pow(x_diff, 2) + pow(y_diff, 2)));

	return heuristic + static_cast<double>(pos->get_depth());
}

/* 
* Get the agent's cost
* @return the cost of the agent (path length)
*/
int Agent::get_cost()
{
	/* Find the solution if it has not been found yet */
	if (goal_node == NULL)
		find_solution();

	/* The cost is equal to the depth of the goal node */
	return goal_node->get_pos()->get_depth();
}

/*
* Destructor
*/
Agent::~Agent()
{
	delete open_list_hash_table;
	delete closed_list;
	delete goal;
	delete start_coord;

#ifndef CBS_CLASSIC
	delete path_clear;
#endif
}