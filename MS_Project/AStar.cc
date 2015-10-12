#include <math.h>
#include <iostream>
#include <fstream>

#include "AStar.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "CantorPair.h"
#include "World.h"
#include "AStarNodeList.h"
#include "AStarNodePointer.h"
#include "PathClearAStar.h"

/* 
* Initialize the A* search and place the root on the OPEN list 
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_world: Pointer to the world this search will navigate
*/
AStar::AStar(Coord* p_start, Coord* p_goal, World* p_world)
{
	/* Get the goal coordinate of the A* Search */
	goal = new Coord(p_goal);

	/* No goal node is found initially */
	goal_node = nullptr;

	/* Initialize lists and hash tables */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>();
	open_list_hash_table = new AStarNodeList(nullptr);
	closed_list = new AStarNodeList(nullptr);
	constraints = std::unordered_map<int, bool>();

	/* Place the root into the OPEN list */
	Position start_pos = Position(p_start, 0);
	AStarNode* start_node = new AStarNode(start_pos, nullptr, calc_cost(&start_pos));
	open_list.emplace(start_node);
	open_list_hash_table->add_node(start_node);

	/* Set the world to navigate */
	world = p_world;

	/* No new constraint */
	path_clear = nullptr;
}

/*
* Constructor that begins by eliminating nodes descended from
* a position given as a parameter
* @param p_astar: The AStar list to copy
* @param new_constraint: The new constraint to remove all descendants of
* from the CLOSED and OPEN lists
*/
AStar::AStar(AStar* p_astar, Position* new_constraint)
{
	/* Get the start and goal coordinate of the A* Search */
	goal = new Coord(*(p_astar->get_goal()));

	/* No goal node is found initially */
	goal_node = nullptr;

	/* Copy both OPEN lists */
	open_list = *(p_astar->get_open_list());
	open_list_hash_table = new AStarNodeList();
	open_list_hash_table->node_copy(p_astar->get_open_list_hash_table());

	/* Copy the constraints and add the new constraint */
	constraints = *(p_astar->get_constraints());
	int hash = CantorPair::get_int(new_constraint);
	constraints.emplace(hash, true);

	/* Copy the closed list as well	*/
	closed_list = new AStarNodeList(nullptr);
	closed_list->node_copy(p_astar->get_closed_list());
	/**********************************************************************
	* UNCOMMENT THIS IF YOU WANT TO GO BACK TO USING PARENT CLOSED LISTS
	closed_list = new AStarNodeList(p_astar->get_closed_list());
	**********************************************************************/

	/* Set the world to navigate */
	world = p_astar->get_world();

	/* Remove descendants of new_constraint from the OPEN and CLOSED lists */
	if (new_constraint != nullptr)
	{
		path_clear = new PathClearAStar(this, new_constraint);
		path_clear->path_clear_a_star();
	}
}

/*
* Add a conflict to the conflict hash table
* @param conflict: Pointer to the position to add to the list of conflicts
*/
void AStar::add_conflict(Position* conflict)
{
	int hash = CantorPair::get_int(conflict);
	constraints.emplace(hash, true);
}

/*
* Perform the A* search and save the goal node as goal_node
*/
void AStar::find_solution()
{
	while (!open_list.empty())
	{
		/* Pop min cost from open_list and remove from hash table */
		AStarNode* heap_top = open_list.top();
		open_list.pop();

		/* Find the node on the OPEN list hash table */
		AStarNodePointer* a_star_ptr;
		AStarNode* check_parent = heap_top->get_parent();
		if (check_parent != nullptr)
			a_star_ptr = open_list_hash_table->check_duplicate(heap_top->get_pos(), check_parent->get_pos());
		else
			a_star_ptr = open_list_hash_table->check_duplicate(heap_top->get_pos(), nullptr);

		/* Make sure the node has not been removed from the list by PathClearA* */
		if (a_star_ptr == nullptr)
			continue;

		/*
		* Use the node in the OPEN list hash table rather than from the minheap because the minheap node
		* points to a node in the parent A* searches OPEN list hash table
		*/
		AStarNode* top = a_star_ptr->get_ptr();

		/* Check if the node is a solution, if it is, return it */
		if (*top->get_pos()->get_coord() == *goal)
		{
			goal_node = top;

			/* Remove the node from the OPEN list without deleting the node */
			open_list_hash_table->remove_hash(top);

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
			AStarNodePointer* check_open_list = open_list_hash_table->check_duplicate(&successors[i], top->get_pos());
			AStarNodePointer* check_closed_list = closed_list->check_duplicate(&successors[i], top->get_pos());
			
			/* If the successor is not a duplicate, add it to the OPEN list */
			if (check_open_list == nullptr && check_closed_list == nullptr)
			{
				/* Create a new node and add it to the OPEN list (both heap and hash table) */
				AStarNode* add_node = new AStarNode(successors[i], top, calc_cost(&successors[i]));
				open_list.push(add_node);
				open_list_hash_table->add_node(add_node);

				/* Increment the counter */
				check_open_list = open_list_hash_table->check_duplicate(&successors[i], top->get_pos());
				check_open_list->inc();
			}
			else if (check_open_list != nullptr)
				check_open_list->inc();
			else if (check_closed_list != nullptr)
				check_closed_list->inc();
		}

		/* Add top to the CLOSED list unless a duplicate is found */
		if (!closed_list->check_duplicate(top))
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
void AStar::get_successors(Position* pos, std::vector<Position>* successors)
{
	/* Get the coordinates of pos */
	int x_coord = pos->get_coord()->get_xcoord();
	int y_coord = pos->get_coord()->get_ycoord();
	
	/* Get the new depth */
	int depth = pos->get_depth() + 1;

	/* Number of possible successors */
	const int NUM_SUCCESSORS = 9;

	/* Create positions for each possible successor */
	Position possible_successors[NUM_SUCCESSORS];
	possible_successors[0] = Position(x_coord + 1, y_coord, depth);
	possible_successors[1] = Position(x_coord + 1, y_coord + 1, depth);
	possible_successors[2] = Position(x_coord + 1, y_coord - 1, depth);
	possible_successors[3] = Position(x_coord - 1, y_coord, depth);
	possible_successors[4] = Position(x_coord - 1, y_coord + 1, depth);
	possible_successors[5] = Position(x_coord - 1, y_coord - 1, depth);
	possible_successors[6] = Position(x_coord, y_coord, depth);
	possible_successors[7] = Position(x_coord, y_coord + 1, depth);
	possible_successors[8] = Position(x_coord, y_coord - 1, depth);

	/* 
	* Add each possible successor if it is an existing coordinate, 
	* is not blocked by an obstacle and is not constrained.
	*/
	for (int i = 0; i < NUM_SUCCESSORS; i++)
	{
		/* Get the hash of the position */
		int hash = CantorPair::get_int(&possible_successors[i]);

		if (
			world->check_coord(possible_successors[i].get_coord()) &&
			constraints.find(hash) == constraints.end()
			)
			successors->push_back(possible_successors[i]);
	}
}

/*
* Return the solution as a stack of coordinates
* @return a stack of Coord objects, if find_solution() has not been
* called yet, call it.
*/
std::stack<Coord> AStar::get_solution()
{
	/* Initialize the stack */
	std::stack<Coord> path = std::stack<Coord>();

	/* Find the solution if it has not been found yet */
	if (goal_node == nullptr)
		find_solution();

	/* Push the goal node's Coord object onto the stack */
	path.push(goal_node->get_pos()->get_coord());

	/* Create a node pointer to traverse the nodes parents */
	AStarNode* trav = goal_node->get_parent();

	while (trav != nullptr)
	{
		path.push(trav->get_pos()->get_coord());
		trav = trav->get_parent();
	}

	return path;
}

/*
* Print the solution to the console 
*/
void AStar::print_solution()
{
	/* Get the solution */
	std::stack<Coord> path = get_solution();

	/* Make sure the solution is correct */
	while (!path.empty())
	{
		std::cout << path.top() << std::endl;
		path.pop();
	}
}

/*
* Calculate the cost of a position
* @param pos: The position to calculate the cost for
* @return the total cost of the position
*/
float AStar::calc_cost(Position* pos)
{
	/* Distances between pos and goal in X and Y axes */
	int x_diff = pos->get_coord()->get_xcoord() - goal->get_xcoord();
	int y_diff = pos->get_coord()->get_ycoord() - goal->get_ycoord();

	float heuristic = sqrt(pow(x_diff, 2) + pow(y_diff, 2));

	return heuristic + float(pos->get_depth());
}

/*
* Destructor
*/
AStar::~AStar()
{
	delete open_list_hash_table;
	delete closed_list;
	delete goal;
	delete path_clear;
	delete goal_node;
}