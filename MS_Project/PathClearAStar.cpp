#include <math.h>

#include "PathClearAStar.h"
#include "Agent.h"
#include "AStarNode.h"
#include "AStarNodePointer.h"
#include "Coordinates.h"
#include "CantorPair.h"
#include "World.h"
#include "AStarNodeList.h"

/*
* Get the goal coordinate from the parent search as well as pointers
* to its OPEN and CLOSED hash tables to be able to remove nodes in those
* hash tables if they are also found in this A* Search
* @param search: An Agent object which this search will remove nodes from
* based on nodes found in this search that also exist in 'search'
* @param start_pos: The starting position (coordinate and depth) of this search
*/
PathClearAStar::PathClearAStar(Agent* search, Position* start_pos)
{
	/* Both searches share the same world */
	world = search->get_world();

	/* Goal coordinate of the agent */
	goal = search->get_goal();

	/* OPEN list in the form of a min heap */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >();

	/* OPEN list in the form of a hash table */
	open_list_hash_table = new AStarNodeList(NULL);

	/* Place the root on the OPEN list */
	AStarNode* root = new AStarNode(*start_pos, NULL, calc_cost(start_pos));
	open_list.emplace(root);

	/* CLOSED list for the search in the form of a hash table */
	closed_list = new AStarNodeList(NULL);

	/*
	* Hash table for the conflicts at this node. The key is the cantor pair of the Position
	* and the value is simply a meaningless boolean value. The value is meaningless because
	* the only thing that matters is whether or not the Position is in the hash table
	*/
	constraints = search->get_constraints();

	/* Pointer to the OPEN hash table of the parent A* Search */
	parent_open_list = search->get_open_list_hash_table();

	/* Pointer to the CLOSED hash table of the parent A* Search */
	parent_closed_list = search->get_closed_list();
}

/* 
* Clear the OPEN and CLOSED lists of descendants of start_pos 
*/
void PathClearAStar::path_clear_a_star()
{
	while (!open_list.empty())
	{
		/* Pop min cost from open_list */
		AStarNode* top = open_list.top();
		open_list.pop();

		/* Check if the node is a solution, if it is, the search is done */
		if (*top->get_pos()->get_coord() == *goal)
			return;

		/* Generate successors */
		std::vector<Position> successors = std::vector<Position>();
		get_successors(top->get_pos(), &successors);

		/* For each successor, check if it is in the OPEN list and CLOSED list */
		int len = successors.size();
		for (int i = 0; i < len; i++)
		{
			/* Make sure the successor is not constrained */
			int hash = CantorPair::get_int(&successors[i]);
			if (constraints->find(hash) != constraints->end())
				continue;

			/* Check if the successor is in either the OPEN or CLOSED list */
			AStarNodePointer* check_open_list = open_list_hash_table->check_duplicate(&successors[i], top->get_pos());
			AStarNodePointer* check_closed_list = closed_list->check_duplicate(&successors[i], top->get_pos());

			/* If the successor is not a duplicate, add it to the OPEN list */
			if (check_open_list == NULL && check_closed_list == NULL)
			{
				/* Create a new node and add it to the OPEN list (both heap and hash table) */
				AStarNode* add_node = new AStarNode(successors[i], top, calc_cost(&successors[i]));
				open_list.push(add_node);
				open_list_hash_table->add_node(add_node);

				/* Remove the node from the OPEN and CLOSED lists of the parent */
				parent_open_list->delete_node(add_node,false);
				parent_closed_list->delete_node(add_node,true);
			}
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
void PathClearAStar::get_successors(Position* pos, std::vector<Position>* successors)
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
	possible_successors[7] = new  Position(x_coord, y_coord + 1, depth);
	possible_successors[8] = new Position(x_coord, y_coord - 1, depth);

	/*
	* Add each possible successor if it is an existing coordinate and
	* is not blocked by an obstacle
	*/
	for (int i = 0; i < NUM_SUCCESSORS; i++)
	{
		/* Get the hash of the position */
		int hash = CantorPair::get_int(possible_successors[i]);

		if (
			world->check_coord(possible_successors[i]->get_coord()) &&
			constraints->find(hash) == constraints->end()
			)
			successors->push_back(*possible_successors[i]);
	}

	/* Possible successors are pushed to successors by value, no need to keep them */
	for (int i = 0; i < NUM_SUCCESSORS; i++)
		delete possible_successors[i];
}

/*
* Calculate the cost of a position
* @param pos: The position to calculate the cost for
* @return the total cost of the position
*/
float PathClearAStar::calc_cost(Position* pos)
{
	/* Distances between pos and goal in X and Y axes */
	int x_diff = pos->get_coord()->get_xcoord() - goal->get_xcoord();
	int y_diff = pos->get_coord()->get_ycoord() - goal->get_ycoord();

	float heuristic = sqrt(pow(x_diff, 2) + pow(y_diff, 2));

	return heuristic + float(pos->get_depth());
}

/* Destructor */
PathClearAStar::~PathClearAStar()
{
	delete open_list_hash_table;
	delete closed_list;
}