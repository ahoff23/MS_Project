#include <math.h>

#include "AStar.h"
#include "AStarNode.h"
#include "Coordinates.h"
#include "CantorPair.h"
#include "World.h"
#include "ClosedList.h"

/* 
* Initialize the A* search and place the root on the OPEN list 
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_world: Pointer to the world this search will navigate
*/
AStar::AStar(Coord* p_start, Coord* p_goal, World* p_world)
{
	/* Get the start and goal coordinate of the A* Search */
	start = new Coord(p_start);
	goal = new Coord(p_goal);

	/* No goal node is found initially */
	goal_node = nullptr;

	/* Initialize lists and hash tables */
	open_list = std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>();
	open_list_hash_table = std::unordered_multimap<int, AStarNode*>();
	constraints = std::unordered_map<int, bool>();

	/* Place the root into the OPEN list */
	Position start_pos = Position(*start, 0);
	add_open(&start_pos, nullptr);

	/* Set the world to navigate */
	world = p_world;
}

/*
* Constructor
* @param p_start: The starting coordinate of the A* search
* @param p_goal: The goal coordinate of the A* search
* @param p_open_list: The OPEN list as a minheap
* @param p_open_list_hash_table: The OPEN list as a hash table
* @param p_constraints: A hash table of the constraints
* @param p_world: Pointer to the world this search will navigate
*/
AStar::AStar(
	Coord* p_start, Coord* p_goal,
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, std::greater<AStarNode>>* p_open_list,
	std::unordered_multimap<int, AStarNode*>* p_open_list_hash_table,
	std::unordered_map<int, bool>* p_constraints,
	World* p_world
	)
{
	/* Get the start and goal coordinate of the A* Search */
	start = new Coord(p_start);
	goal = new Coord(p_goal);

	/* No goal node is found initially */
	goal_node = nullptr;

	/* Copy both OPEN lists */
	open_list = *p_open_list;
	open_list_hash_table = *p_open_list_hash_table;

	/* Copy the constraints */
	constraints = *p_constraints;

	/* Set the world to navigate */
	world = p_world;
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
		//Pop min cost from open_list and remove from hash table
		AStarNode* top = open_list.top();
		open_list.pop();
		open_list_hash_table.erase(CantorPair::get_int(top->get_pos()));

		//Check if the node is a solution, if it is, return it
		if (top->get_pos()->get_coord() == goal)
		{
			goal_node = top;
			return;
		}

		//Generate successors
		std::vector<Position> successors = std::vector<Position>();
		get_successors(top->get_pos(), &successors);

		//For each successor, check if it is in the open_list
		int len = successors.size();
		for (int i = 0; i < len; i++)
		{
			int check_open_list = check_duplicate_open(&successors[i], top->get_pos());

			/* If it is and its parent is there as well, do nothing */
			if (check_open_list == 2)
				continue;
			/* 
			* If it is but its parent node is not there or if it is not
			* there at all, add the parent to that node 
			*/
			else if (check_open_list == 1 || check_open_list == 0)
			{
				int hash = CantorPair::get_int(&successors[i]);
				open_list_hash_table.emplace(hash, top);
			}
		}

		/* Add top to the CLOSED list unless a duplicate is found */
		if (!closed_list->check_duplicate(top))
			closed_list->add_node(top);
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
	* Add each possible successor if it is an existing coordinate and 
	* is not blocked by an obstacle
	*/
	for (int i = 0; i < NUM_SUCCESSORS; i++)
	{
		if (world->check_coord(possible_successors[i].get_coord()))
			successors->push_back(possible_successors[i]);
	}
}

/* 
* Check if a node is in the OPEN list 
* @param pos: The Position to check
* @param parent: The parent of pos (i.e. the position directly preceding pos)
* @return 0 if pos is not in the list, 1 if it is but the parent is not, 
* 2 if it is in the list and has a parent equal to the parent parameter
*/
int AStar::check_duplicate_open(Position* pos, Position* parent)
{
	/* Get the hash of the Position via a Cantor Pair */
	int hash = CantorPair::get_int(pos);

	/* Check if the position is in the hash table at all */
	auto it = open_list_hash_table.find(hash);
	if (it == open_list_hash_table.end())
		return 0;

	/* Check if the parameter parent is a parent of pos in the hash table */
	while (it != open_list_hash_table.end() && it->first == hash)
	{
		if (*it->second->get_pos() == *parent)
			return 2;
	}

	return 1;
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