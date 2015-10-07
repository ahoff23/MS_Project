#include <math.h>

#include "AStar.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "CantorPair.h"

/* 
* Initialize the A* search and place the root on the OPEN list 
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
*/
AStar::AStar(Coord* p_start, Coord* p_goal)
{
	/* Get the start and goal coordinate of the A* Search */
	start = new Coord(p_start);
	goal = new Coord(p_goal);

	/* Initialize lists and hash tables */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>();
	open_list_hash_table = std::unordered_map<int, AStarNode*>();
	constraints = std::unordered_map<int, bool>();

	/* Place the root into the OPEN list */
	Position start_pos = Position(*start, 0);
	add_open(&start_pos, nullptr);
}

/*
* Constructor
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_open_list: The OPEN list as a minheap
* @param p_open_list_hash_table: The OPEN list as a hash table
* @param p_constraints: A hash table of the constraints
*/
AStar::AStar(
	Coord* p_start, Coord* p_goal,
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>* p_open_list,
	std::unordered_map<int, AStarNode*>* p_open_list_hash_table,
	std::unordered_map<int, bool>* p_constraints
	)
{
	/* Get the start and goal coordinate of the A* Search */
	start = new Coord(p_start);
	goal = new Coord(p_goal);

	/* Copy both OPEN lists */
	open_list = *p_open_list;
	open_list_hash_table = *p_open_list_hash_table;

	/* Copy the constraints */
	constraints = *p_constraints;
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
* Perform the A* search and return the goal node. This function
* places the responsibility of de-allocating the dynamic memory of the
* return node on the function caller.
*/
AStarNode * AStar::find_solution()
{
	while (!open_list.empty())
	{
		//Pop min cost from open_list and remove from hash table

		//Check if the node is a solution, if it is, return it

		//Generate successors

		//For each successor, check if it is in the open_list
			//If it is and its parent is there as well, do nothing
			//If it is but its parent node is not there, add the parent to that node
			//If it is not, construct a new node and add it to the list and hash table

		//Check if the popped node is a duplicate
			//If it is and its parent is already in the closed list, do nothing
			//If it is but its parent is unique, add the parent
			//If it is not, add the node to the closed list
	}
}

/* 
* Place a node in the OPEN list 
* @param pos: The position to add to the OPEN lists
* @param parent: The parent of the new node
*/
void AStar::add_open(Position* pos, AStarNode* parent)
{
	/* Create a new AStarNode */
	AStarNode* add_node = new AStarNode(pos, parent, calc_cost(pos));

	/* Place the node in both the minheap and the hash table */
	open_list.emplace(add_node);
	open_list_hash_table.emplace(CantorPair::get_int(pos), add_node);
}

/* 
* Calculate the cost of a position 
* @param pos: The position to calculate the cost for
* @return the total cost of the position
*/
float AStar::calc_cost(Position* pos)
{
	float heuristic = sqrt(pow(pos->get_coord()->get_xcoord() - goal->get_xcoord(), 2)
		+ pow(pos->get_coord()->get_ycoord() - goal->get_ycoord(), 2));

	return heuristic + float(pos->get_depth());
}

/*
* Destructor
*/
AStar::~AStar()
{
	delete closed_list;
	delete start;
	delete goal;

	/* Delete every OPEN list node */
	while (!open_list.empty())
	{
		delete open_list.top();
		open_list.pop();
	}
}