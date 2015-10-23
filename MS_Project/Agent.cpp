#include <math.h>
#include <iostream>
#include <fstream>

#include "Agent.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "CantorPair.h"
#include "World.h"
#include "AStarNodeList.h"
#include "PathClearAStar.h"

/* 
* Initialize the agent search and place the root on the OPEN list 
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_world: Pointer to the world this search will navigate
* @param p_name: The agent's name
*/
Agent::Agent(Coord* p_start, Coord* p_goal, World* p_world, std::string p_name)
{
	/* Get the goal coordinate of the A* Search */
	goal = new Coord(p_goal);

	/* No goal node is found initially */
	goal_node = NULL;

	/* Store the start coord */
	start_coord = new Coord(p_start);

	/* Initialize lists and hash tables */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();
	open_list_hash_table = new AStarNodeList(NULL);
	closed_list = new AStarNodeList(NULL);
	constraints = std::unordered_map<int, bool, hash_struct>();

	/* Place the root into the OPEN list */
	Position start_pos = Position(p_start, 0);
	AStarNode* start_node = new AStarNode(start_pos, NULL, calc_cost(&start_pos));
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
	closed_list = new AStarNodeList(NULL);

	/* Copy the constraints and add the new constraint */
	constraints = *(p_agent->get_constraints());
	int hash = CantorPair::get_int(new_constraint);
	constraints.emplace(hash, true);

/* Using CBS Classic (no PCA*) */
#ifdef CBS_CLASSIC
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();

	/* Put the start node in the OPEN list */
	Position start_pos = Position(p_agent->get_start(), 0);
	AStarNode* start_node = new AStarNode(start_pos, NULL, calc_cost(&start_pos));
	open_list.emplace(start_node);
	open_list_hash_table->add_node(start_node);

/* Using PCA* */
#else CBS_CLASSIC
	/* Copy both OPEN lists */
	open_list = *(p_agent->get_open_list());
	open_list_hash_table->node_copy(p_agent->get_open_list_hash_table());

	/* Copy the closed list as well	*/
	closed_list->node_copy(p_agent->get_closed_list());
	/**********************************************************************
	* UNCOMMENT THIS IF YOU WANT TO GO BACK TO USING PARENT CLOSED LISTS
	closed_list = new AStarNodeList(p_astar->get_closed_list());
	**********************************************************************/

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
}

/*
* Add a conflict to the conflict hash table
* @param conflict: Pointer to the position to add to the list of conflicts
*/
void Agent::add_conflict(Position* conflict)
{
	int hash = CantorPair::get_int(conflict);
	constraints.emplace(hash, true);
}

/*
* Perform the A* search and save the goal node as goal_node
*/
void Agent::find_solution()
{
#ifdef OPEN_LIST_DATA
	/* Create a variable for the number of nodes popped from the OPEN list*/
	int popped = 0;
	/* Create a variable for the number of nodes added to the OPEN list*/
	int added = 0;
#endif

	while (!open_list.empty())
	{
		/* Pop min cost from open_list and remove from hash table */
		AStarNode* heap_top = open_list.top();
		open_list.pop();

#ifdef OPEN_LIST_DATA
		/* Increment popped counter */
		popped++;
#endif

		/* Find the node on the OPEN list hash table */
		AStarNode* top;
		AStarNode* check_parent = heap_top->get_parent();
		if (check_parent != NULL)
			top = open_list_hash_table->check_duplicate(
				heap_top->get_pos(), check_parent->get_pos()
				);
		else
			top = open_list_hash_table->check_duplicate(
				heap_top->get_pos(), NULL
				);

		/* Make sure the node has not been removed from the list by PathClearA* */
		if (top == NULL)
			continue;

		/* Check if the node is a solution, if it is, return it */
		if (*top->get_pos()->get_coord() == *goal)
		{
			goal_node = top;

			/* Remove the node from the OPEN list without deleting the node */
			open_list_hash_table->remove_hash(top);

#ifdef OPEN_LIST_DATA
			/* Print the OPEN list stats */
			std::cout << "AGENT DEPTH: " << agent_depth << std::endl;
			std::cout << "SIZE OF ALL OPEN LIST NODES: " << open_list.size() - added + popped << std::endl;
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
			/* Make sure the successor is not constrained */
			int hash = CantorPair::get_int(&successors[i]);
			if (constraints.find(hash) != constraints.end())
				continue;

			/* Check if the successor is in either the OPEN or CLOSED list */
			AStarNode* check_open_list = open_list_hash_table->check_duplicate(&successors[i], top->get_pos());
			AStarNode* check_closed_list = closed_list->check_duplicate(&successors[i], top->get_pos());
			
			/* If the successor is not a duplicate, add it to the OPEN list */
			if (check_open_list == NULL && check_closed_list == NULL)
			{
				/* Create a new node and add it to the OPEN list (both heap and hash table) */
				AStarNode* add_node = new AStarNode(successors[i], top, calc_cost(&successors[i]));
				open_list.push(add_node);
				open_list_hash_table->add_node(add_node);

#ifdef OPEN_LIST_DATA
				/* Increment added counter */
				added++;
#endif
			}
			else if (check_open_list != NULL)
				open_list_hash_table->add_node(check_open_list);
			else if (check_closed_list != NULL)
				closed_list->add_node(check_closed_list);
		}

		/* Add top to the CLOSED list */
		closed_list->add_node(top);

		/* Remove the node from the OPEN list without deleting the node */
		open_list_hash_table->remove_hash(top);
	}
}

/* 
* Get a vector of successor positions of a given position 
* @param pos: The position whose successors will be found by this function
* @param successors: Vector which successors will be appended to
*/
void Agent::get_successors(Position* pos, std::vector<Position>* successors)
{
	/* Get the coordinates of pos */
	int x_coord = pos->get_coord()->get_xcoord();
	int y_coord = pos->get_coord()->get_ycoord();
	
	/* Get the new depth */
	int depth = pos->get_depth() + 1;

	/* Number of possible successors */
	const int NUM_SUCCESSORS = 9;

	/* Create positions for each possible successor */
	Position* possible_successors[NUM_SUCCESSORS];
	possible_successors[0] = new Position(x_coord + 1, y_coord, depth);
	possible_successors[1] = new Position(x_coord + 1, y_coord + 1, depth);
	possible_successors[2] = new Position(x_coord + 1, y_coord - 1, depth);
	possible_successors[3] = new Position(x_coord - 1, y_coord, depth);
	possible_successors[4] = new Position(x_coord - 1, y_coord + 1, depth);
	possible_successors[5] = new Position(x_coord - 1, y_coord - 1, depth);
	possible_successors[6] = new Position(x_coord, y_coord, depth);
	possible_successors[7] = new Position(x_coord, y_coord + 1, depth);
	possible_successors[8] = new Position(x_coord, y_coord - 1, depth);

	/* 
	* Add each possible successor if it is an existing coordinate, 
	* is not blocked by an obstacle and is not constrained.
	*/
	for (int i = 0; i < NUM_SUCCESSORS; i++)
	{
		/* Get the hash of the position */
		int hash = CantorPair::get_int(possible_successors[i]);

		if (
			world->check_coord(possible_successors[i]->get_coord()) &&
			constraints.find(hash) == constraints.end()
			)
			successors->push_back(*possible_successors[i]);
	}

	/* Delete all possible successors (pushed by to return list of successors by value) */
	for (int i = 0; i < NUM_SUCCESSORS; i++)
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

	/* Find the solution if it has not been found yet */
	if (goal_node == NULL)
		find_solution();

	/* Push the goal node's Coord object onto the stack */
	path.push(goal_node->get_pos()->get_coord());

	/* Create a node pointer to traverse the nodes parents */
	AStarNode* trav = goal_node->get_parent();

	while (trav != NULL)
	{
		path.push(trav->get_pos()->get_coord());
		trav = trav->get_parent();
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
	file << "*********************" << "\n";
	file << name << "\n";
	file << "*********************" << "\n";

	/* Make sure the solution is correct */
	while (!path.empty())
	{
		file << path.top() << "\n";
		path.pop();
	}

	/* Add an extra line for formatting */
	file << "\n";
}

/*
* Calculate the cost of a position
* @param pos: The position to calculate the cost for
* @return the total cost of the position
*/
float Agent::calc_cost(Position* pos)
{
	/* Distances between pos and goal in X and Y axes */
	int x_diff = pos->get_coord()->get_xcoord() - goal->get_xcoord();
	int y_diff = pos->get_coord()->get_ycoord() - goal->get_ycoord();

	float heuristic = sqrt(pow(x_diff, 2) + pow(y_diff, 2));

	return heuristic + float(pos->get_depth());
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

	/* Create a node pointer to traverse the nodes parents */
	AStarNode* trav = goal_node->get_parent();

	/* Agent cost */
	int cost = 1;

	/* Backtrack through each coordinate in the path */
	while (trav != NULL)
	{
		trav = trav->get_parent();
		cost++;
	}

	return cost;
}

/*
* Destructor
*/
Agent::~Agent()
{
	delete open_list_hash_table;
	delete closed_list;
	delete goal;
	delete path_clear;
	delete goal_node;
	delete start_coord;
}