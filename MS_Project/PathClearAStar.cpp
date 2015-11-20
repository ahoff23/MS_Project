#include <math.h>
#include <queue>

#include "PathClearAStar.h"
#include "Agent.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "World.h"
#include "AStarNodeList.h"
#include "AStarNodeMultiMap.h"
#include "Macros.h"
#include "Exceptions.h"
#include "HashStruct.h"

#ifdef PCA_STAR_SIZE
#include <iostream>
#endif

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

	/* Remove the constraint from both the OPEN and CLOSED list of the parent A* search */
	parent_open_list->remove_hash(start_pos);
	parent_closed_list->remove_hash(start_pos);

	/*
	* If the constraint is the goal node, then it will be at the top of the OPEN list.
	* Remove it.
	*/
	if (*start_pos->get_coord() == *goal)
	{
		std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode> >*
			parent_open_heap = search->get_open_list();
		if (!(*parent_open_heap->top()->get_pos() == *start_pos))
			throw TerminalException("Top of heap should be goal but was not.");
		parent_open_heap->pop();
	}

	/* Get the name of the agent */
	name = search->get_name();
}

/* 
* Clear the OPEN and CLOSED lists of descendants of start_pos 
* @return true if the goal node is removed, false if it is not
* (a path still exists to the goal node)
*/
#include <iostream>
bool PathClearAStar::path_clear_a_star()
{
	while (!open_list.empty())
	{
		/* Get the minimum cost node in the list */
		AStarNode* top = open_list.top();
		open_list.pop();



//		std::cout << std::endl << "***************************" << std::endl;
//		std::cout << *top->get_pos() << " with cost " << top->get_cost() << std::endl;
//		std::cout << "***************************" << std::endl;
	
		/* Check if the node is a solution, if it is, the search is done */
		if (*top->get_pos()->get_coord() == *goal)
		{
#ifdef PCA_STAR_SIZE
			/* Display the size of the OPEN and CLOSED lists */
			std::cout << "PCA* OPEN LIST SIZE: " << open_list.size() << std::endl;
			std::cout << "PCA* CLOSED LIST SIZE:" << closed_list->get_size() << std::endl;
#endif
			remove_extra_open_nodes();
			return true;
		}

		/* Generate successors */
		std::vector<Position> successors;
		get_successors(top->get_pos(), &successors);

		/* For each successor, check if it is in the OPEN or CLOSED list */
		int len = successors.size();
		for (int i = 0; i < len; i++)
		{


	//		std::cout << *successors[i].get_coord() << " with cost " << calc_cost(&successors[i]) << std::endl;


			/*
			* Remove the node from the OPEN or CLOSED list of the parent.
			* If no parent is removed then do not generate this node.
			*/
			if (!del_successors(&successors[i], top->get_pos()->get_coord()))
				continue;

			/* Create a new node and add it to the OPEN list (both heap and hash table) */
			AStarNode* add_node = new AStarNode(successors[i], top, calc_cost(&successors[i]));
			open_list.push(add_node);
			open_list_hash_table->add_node(add_node);
		}
		/* Add top to the CLOSED list unless a duplicate is found */
		closed_list->add_node(top);

		/* Remove the node from the OPEN list without deleting the node */
		open_list_hash_table->remove_hash(top);
	}
	return false;
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
		if (
			world->check_coord(possible_successors[i]->get_coord()) &&
			constraints->find(HashStruct::hash_pos(possible_successors[i])) == constraints->end()
			)
			successors->push_back(*possible_successors[i]);
	}

	/* Possible successors are pushed to successors by value, no need to keep them */
	for (int i = 0; i < NUM_SUCCESSORS; i++)
		delete possible_successors[i];
}

/*
* Delete a parent from successor node
* @param pos: The position to deccrement
* @param parent: The parent coordinate to remove
* @return true if the position is still expandable (deleted from the CLOSED list)
* and false if it is not (found on the OPEN list or decremented from the CLOSED list).
* Also return false if this is the first time the goal node has been found.
* If the position is not found in either list, throw an exception.
*/
bool PathClearAStar::del_successors(Position* pos, Coord* parent)
{
	/* Attempt to delete the node from the OPEN list */
	int open_result = parent_open_list->delete_node(pos, parent, false);

	/* If the position is the first time the goal node has been reached, return false */
	if (*pos->get_coord() == *goal)
	{
		if (!open_list_hash_table->check_duplicate(pos))
			return true;
		else
			return false;
	}

	/* If the node is in the A* OPEN list, do not add it to the PCA* OPEN list */
	if (open_result != 0)
		return false;

	/* Check the A* CLOSED list */
	int closed_result = parent_closed_list->delete_node(pos, parent, true);

	/* If the node cannot be found in either A* list, throw an error */
	if (closed_result == 0)
	{


		std::cout << "POS: " << *pos << std::endl;
		std::cout << "PARENT: " << *parent << std::endl;
		std::cout << "GOAL: " << *goal << std::endl;
		std::cin.get();


		throw TerminalException(
			"Attempted to delete node in neither A* list."
			);
	}
	/*
	* If the node is found but not deleted,
	* do not add it to the PCA* OPEN list
	*/
	else if (closed_result == 1)
		return false;
	return true;
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

	float heuristic = static_cast<float>(sqrt(pow(x_diff, 2) + pow(y_diff, 2)));

	return heuristic + float(pos->get_depth());
}


/*
* Remove nodes from the OPEN list that were missed due a slightly different ordering
*/
void PathClearAStar::remove_extra_open_nodes()
{
	/* Create a vector to store the successors of nodes deleted in the OPEN list */
	std::queue<Position> unexpanded;

	/* Get the OPEN list as a hash table */
	std::unordered_multimap<unsigned int, AStarNode*>* map = 
		open_list_hash_table->get_list()->get_map();

	/* Each node in the PCA* OPEN list is not in the A* OPEN list and must be deleted */
	for (auto it = map->begin(); it != map->end(); it++)
	{
		Position* check_pos = it->second->get_pos();

		/* Skip the goal node */
		if (*check_pos->get_coord() == *goal)
			continue;

		/* Get the successor nodes */
		std::vector<Position> successors;
		get_successors(check_pos, &successors);

		for (int i = 0; i < successors.size(); i++)
			dec_push_unexpanded(&successors[i], check_pos->get_coord(), &unexpanded);
	}

	/* For each successor node */
	while (unexpanded.size() != 0)
	{
		Position check_pos = unexpanded.front();
		unexpanded.pop();

		/* Skip the goal node */
		if (*check_pos.get_coord() == *goal)
			continue;

		/* Get the successor nodes */
		std::vector<Position> successors;
		get_successors(&check_pos, &successors);

		for (int i = 0; i < successors.size(); i++)
			dec_push_unexpanded(&successors[i], check_pos.get_coord(), &unexpanded);
	}
}

/*
* Decrement and add a successor to the list of unexpanded Positions 
* @param pos: The position to decrement the parent from
* @param parent: The parent to remove
* @param unexpanded: The list of unexpanded nodes to add the
* position to if it is deleted from the CLOSED list
*/
void PathClearAStar::dec_push_unexpanded(
	Position* pos, Coord* parent, std::queue<Position>* unexpanded
	)
{
	/*
	* Decrement the successor node based on this parent.
	* If the node is in the A* OPEN list or it is not deleted,
	* move on to the next node.
	*/
	if (!del_successors(pos, parent))
		return;

	/* Add the node deleted from the CLOSED list to the list of successor nodes */
	unexpanded->push(*pos);
}

/* 
* Destructor 
*/
PathClearAStar::~PathClearAStar()
{
	delete open_list_hash_table;
	delete closed_list;
}