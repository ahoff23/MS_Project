#include <iostream>
#include <fstream>

#include "Tests.h"
#include "Coordinates.h"
#include "AStarNode.h"
#include "AStarNodeList.h"
#include "World.h"
#include "Exceptions.h"
#include "Agent.h"
#include "CBSNode.h"
#include "CBSTree.h"
#include "Macros.h"

/* 
* Runs all tests 
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::run_tests()
{
	if (!coordinate_tests())
		return false;
	else
		std::cout << "Coordinate Tests Passed." << std::endl;

	if (!position_tests())
		return false;	
	else
		std::cout << "Position Tests Passed." << std::endl;

	if (!closed_list_tests())
		return false;
	else
		std::cout << "Closed List Tests Passed." << std::endl;

	if (!world_tests())
		return false;
	else
		std::cout << "World Tests Passed." << std::endl;

	if (!a_star_tests())
		return false;
	else
		std::cout << "A* Tests Passed." << std::endl;

#ifndef CBS_CLASSIC
	if (!path_clear_a_star_tests())
		return false;
	else
		std::cout << "Path Clear A* Tests Passed." << std::endl;
#endif

	if (!cbs_node_tests())
		return false;
	else
		std::cout << "CBSNode Tests Passed." << std::endl;

	if (!cbs_tree_tests())
		return false;
	else
		std::cout << "CBSTree Tests Passed." << std::endl;

	std::cout << "All tests passed." << std::endl;
	return true;
}

/*
* Tests for the Coordinate object
* @return true if all tests pass or print an error and return false if one test fails
*/
bool Tests::coordinate_tests()
{
	/* Test equals operator for Coordinate */
	int x = 1;
	int y = 1;
	Coord coord_1 = Coord(x, y);
	Coord coord_2 = coord_1;

	if (!coordinate_equality(&coord_1, &coord_2))
	{
		std::cout << "FAILED: Coordinate equals operator." << std::endl;
		return false;
	}

	return true;		
}

/*
* Tests for the Position object
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::position_tests()
{

	/* Test equals operator for Position */
	int x = 1;
	int y = 1;
	Coord coord_1 = Coord(x, y);
	int depth = 10;
	Position pos_1 = Position(coord_1, depth);
	Position pos_2 = pos_1;

	if (!coordinate_equality(pos_1.get_coord(), pos_2.get_coord()) || pos_1.get_depth() != pos_2.get_depth())
	{
		std::cout << "FAILED: Position equals operator." << std::endl;
		return false;
	}

	return true;
}

/*
* Auxiliary function which determines if two coordinates are equal
* @param coord_1: Pointer to a coordinate for equality test
* @param coord_2: Pointer to a coordinate for equality test
* @return true if coord_1 and coord_2 are equal, false otherwise
*/
bool Tests::coordinate_equality(Coord* coord_1, Coord* coord_2)
{
	if (
		coord_1->get_xcoord() != coord_2->get_xcoord() || 
		coord_1->get_ycoord() != coord_2->get_ycoord()
		)
		return false;
	return true;
}

/*
* Test ClosedList functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::closed_list_tests()
{
	/* Objects for use in the tests */
	int xcoord = 1;
	int ycoord = 2;
	int depth = 3;
	Position pos_1 = Position(xcoord, ycoord, depth);
	AStarNode* add_node_1 = new AStarNode(pos_1, NULL, 1);

	/* Add a ClosedNode to the list */
	AStarNodeList list_1 = AStarNodeList();
	list_1.add_node(add_node_1);

	/* Check if a duplicate of the ClosedNode is in the list */
	if (!list_1.check_duplicate(add_node_1))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function did not find an existing position." <<
			std::endl;
		delete add_node_1;
		return false;
	}

	/* Check if a non-duplicate is not in the list */
	Position pos_2 = Position(xcoord + 1, ycoord + 1, depth);
	AStarNode* add_node_2 = new AStarNode(pos_2, NULL, 1);
	if (list_1.check_duplicate(add_node_2))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function found a non-existant duplicate." <<
			std::endl; 
		delete add_node_1;
		delete add_node_2;
		return false;
	}

	/* Create a new ClosedList, with the old ClosedList as its parent */
	AStarNodeList list_2 = AStarNodeList(&list_1);

	/* Add a ClosedNode to the new list */
	list_2.add_node(add_node_2);

	/* Check for a duplicate from the parent */	
	/* Currently commented out because parents are not used */
	/*
	if (!list_1.check_duplicate(add_node_2))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function did not find an existing position" <<
			" in a parent ClosedList." << std::endl;
		delete add_node_1;
		delete add_node_2;
		return false;
	}
	*/

	return true;
}

/*
* Test World functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::world_tests()
{
	/* Pointer to worlds being tested */
	World* test_world;

	/* Error code from an exception */
	std::string exception_msg = "";

	/* Make sure an error occurs when referencing a non-existant file */
	try
	{
		test_world = new World("Worlds/fake_file.txt");
	}
	catch (TerminalException& ex)
	{
		exception_msg = ex.what();
		
		/* Allocate for a new test_world so the delete works later */
		test_world = new World();
	}

	if (exception_msg == "World file could not be opened.")
		exception_msg = "";
	else
	{
		std::cout << "FAILED: Non-existant file did not generate the correct exception." << std::endl;
		return false;
	}

	delete test_world;

	/* Make sure an error is thrown when referencing an empty file */
	char test_file[] = "Worlds/test_file.txt";
	std::ofstream empty_file(test_file);

	try
	{
		test_world = new World(test_file);
	}
	catch (TerminalException& ex)
	{
		exception_msg = ex.what();

		/* Allocate for a new test_world so the delete works later */
		test_world = new World();
	}
	if (exception_msg == "World text file must have at least one coordinate.")
		exception_msg = "";
	else
	{
		std::cout << "FAILED: Empty file did not generate the correct exception." << std::endl;

		/* Remove the test file */
		std::remove(test_file);

		return false;
	}
	empty_file.close();

	delete test_world;

	/* Test incorrect file format */
	empty_file.open(test_file);
	empty_file << "10101 0101";
	empty_file.close();

	try
	{
		test_world = new World(test_file);
	}
	catch (TerminalException& ex)
	{
		exception_msg = ex.what();

		/* Allocate for a new test_world so the delete works later */
		test_world = new World();
	}
	if (exception_msg == "Invalid file format.")
		exception_msg = "";
	else
	{
		std::cout << exception_msg << std::endl;
		std::cout << "FAILED: Invalid format file did not generate the correct exception." << std::endl;

		/* Remove the test file */
		std::remove(test_file);

		return false;
	}

	delete test_world;

	/* Create a world and make sure the check_coord function works */
	empty_file.open(test_file);
	empty_file << "0111100\n";
	empty_file << "0011110\n";
	empty_file << "0001111\n";
	empty_file.close();

	test_world = new World(test_file);

	/* Position that should be empty */
	Coord empty_coord(1, 0);
	if (!test_world->check_coord(&empty_coord))
	{
		std::cout << "FAILED: Coord that should be empty is not." << std::endl;

		/* Remove the test file */
		std::remove(test_file);

		return false;
	}

	/* Position that should be blocked */
	Coord blocked_coord(0, 0);
	if (test_world->check_coord(&blocked_coord))
	{
		std::cout << "FAILED: Coord that should be blocked is not." << std::endl;

		/* Remove the test file */
		std::remove(test_file);

		return false;
	}

	/* Coordinates that should not exist */
	const int NUM_COORDS = 6;
	Coord* coords[NUM_COORDS];
	coords[0] = new Coord(-1, 0);
	coords[1] = new Coord(0, -1);
	coords[2] = new Coord(-1, -1);
	coords[3] = new Coord(0, 3);
	coords[4] = new Coord(7, 0);
	coords[5] = new Coord(7, 3);

	for (int i = 0; i < NUM_COORDS; i++)
	{
		if (test_world->check_coord(coords[i]))
		{
			std::cout << 
				"FAILED: Coord[" << i <<
				"] should not exist, but it is treated as though exists." <<
				std::endl;

			/* Remove the test file */
			std::remove(test_file);

			/* Clean up */
			for (int j = 0; j < NUM_COORDS; j++)
				delete coords[j];

			return false;
		}
	}

	/* Clean up */
	for (int j = 0; j < NUM_COORDS; j++)
		delete coords[j];
	delete test_world;

	/* Remove the test file */
	std::remove(test_file);

	return true;
}

/* 
* Create a world and print it to make sure the world is correctly created 
*/
void Tests::print_world_test(std::string test_file)
{
	/* Create a world based on a text file */
	World* test_world;
	try
	{
		test_world = new World(test_file);
	}
	catch (TerminalException& ex)
	{
		std::cout << ex.what() << std::endl;
		return;
	}

	/* Print the world and manually confirm that it is accurate */
	test_world->print_world();

	delete test_world;
}

/*
* A* functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::a_star_tests()
{
	/* Create a world */
	World* test_world = create_world();

	/* Create the start and goal coordinates */
	Coord start = Coord(0, 0);
	Coord goal = Coord(2, 0);

	/* Create the A* Search */
#ifdef TIME_LIMIT
	Agent* search = new Agent(&start, &goal, test_world, "agent_name", std::clock());
#else
	Agent* search = new Agent(&start, &goal, test_world, "agent_name");
#endif

	/* Get the solution */
	std::stack<Coord> path = search->get_solution();

	/* Validate solution */
	const int SOLUTION_LENGTH = 3;
	Coord check[SOLUTION_LENGTH];
	for (int i = 0; i < SOLUTION_LENGTH; i++)
	{
		/* Make sure the path is not too short */
		if (path.size() == 0)
		{
			std::cout << "FAILED: A* return path is too short." << std::endl;
			return false;
		}

		check[i] = path.top();
		path.pop();
	}

	/* Validate the path length */
	if (path.size() > 0)
	{
		std::cout << "FAILED: A* return path is too long." << std::endl;
		return false;
	}

	/* Validate each path member */
	const int NUM_COORDS = 3;
	Coord** coords = new Coord*[NUM_COORDS];
	coords[0] = new Coord(0, 0);
	coords[1] = new Coord(1, 0);
	coords[2] = new Coord(2, 0);
	if (check[0] != *coords[0] || check[1] != *coords[1] || check[2] != *coords[2])
	{
		std::cout << "FAILED: A* solution path is incorrect" << std::endl;

		/* Clean up */
		for (int i = 0; i < NUM_COORDS; i++)
			delete coords[i];
		delete[] coords;

		return false;
	}

	/* Clean up */
	for (int i = 0; i < NUM_COORDS; i++)
		delete coords[i];
	delete[] coords;
	delete search;

	return true;
}

/*
* PCA* functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::path_clear_a_star_tests()
{
	/* Create a world */
	World* test_world = create_world();

	/* Create the start and goal coordinates */
	Coord start = Coord(0, 0);
	Coord goal = Coord(2, 0);

	/* Create the A* Search and find the solution */
#ifdef TIME_LIMIT
	Agent* search = new Agent(&start, &goal, test_world, "agent_name",std::clock());
#else
	Agent* search = new Agent(&start, &goal, test_world, "agent_name");
#endif
	search->find_solution();

	/* 
	* Create a child search of the prior A* search with a constraint 
	* that would require the original solution change
	*/
	Position constraint = Position(1, 0, 1);
	Agent* constrained_search = new Agent(search, &constraint);

	/* Get the solution */
	std::stack<Coord> path = constrained_search->get_solution();

	/* Validate solution */
	const int SOLUTION_LENGTH = 4;
	Coord check[SOLUTION_LENGTH];
	for (int i = 0; i < SOLUTION_LENGTH; i++)
	{
		/* Make sure the path is not too short */
		if (path.size() == 0)
		{
			std::cout << "FAILED: A* return path is too short." << std::endl;
			delete search;
			delete constrained_search;
			delete test_world;
			return false;
		}

		check[i] = path.top();
		path.pop();
	}

	/* Validate the path length */
	if (path.size() > 0)
	{
		std::cout << "FAILED: A* return path is too long." << std::endl;
		delete search;
		delete constrained_search;
		delete test_world;
		return false;
	}

	delete search;

	/* Validate each path member */
	const int NUM_COORDS = 3;
	Coord** coords = new Coord*[NUM_COORDS];
	coords[0] = new Coord(0, 0);
	coords[1] = new Coord(1, 0);
	coords[2] = new Coord(2, 0);
	if (
		check[0] != *coords[0] || check[1] != *coords[0] ||
		check[2] != *coords[1] || check[3] != *coords[2]
		)
	{
		std::cout << "FAILED: A* solution path is incorrect" << std::endl;

		/* Clean up */
		delete search;
		delete constrained_search;
		delete test_world;
		for (int i = 0; i < NUM_COORDS; i++)
			delete coords[i];
		delete [] coords;
		return false;
	}

	/* Clean up */
	for (int i = 0; i < NUM_COORDS; i++)
		delete coords[i];
	delete [] coords;
	delete constrained_search;
	delete test_world;

	return true;
}

/* 
* Create a 1x3 world with no obstacles 
* @return a pointer to a 1x3 world with no obstacles 
*/
World* Tests::create_world()
{
	/* Pointer to worlds being tested */
	World* test_world;

	/* Create a world and confirm the path is correct */
	char test_file[] = "Worlds/test_file.txt";
	std::ofstream empty_file(test_file);
	empty_file << "111";
	empty_file.close();

	/* Create the new test world */
	try
	{
		test_world = new World(test_file);
	}
	catch (TerminalException& ex)
	{
		std::cout << ex.what() << std::endl;

		/* Remove the test file */
		std::remove(test_file);

		return NULL;
	}

	/* Remove the test file */
	std::remove(test_file);

	return test_world;
}

/*
* CBSNode Functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::cbs_node_tests()
{
	/* Create a world */
	World* test_world = create_world();

	/* Create first agent */
	Coord start_1 = Coord(0, 0);
	Coord goal_1 = Coord(2, 0);
	std::string name_1 = "Agent 1";
#ifdef TIME_LIMIT
	Agent* a_1 = new Agent(&start_1, &goal_1, test_world, name_1,std::clock());
#else
	Agent* a_1 = new Agent(&start_1, &goal_1, test_world, name_1);
#endif

	/* Create second agent */
	Coord start_2 = Coord(2, 0);
	Coord goal_2 = Coord(0, 0);
	std::string name_2 = "Agent 2";
#ifdef TIME_LIMIT
	Agent* a_2 = new Agent(&start_2, &goal_2, test_world, name_2,std::clock());
#else
	Agent* a_2 = new Agent(&start_2, &goal_2, test_world, name_2);
#endif

	/* Create a vector containing both agents */
	std::vector<Agent*> agents = std::vector<Agent*>();
	agents.push_back(a_1);
	agents.push_back(a_2);

	/* Create a CBSNode */
	CBSNode* node = new CBSNode(&agents);

	/* Create variables to store conflict information */
	int agent_1;
	Position* conflict_1 = new Position();
	int agent_2;
	Position* conflict_2 = new Position();

	/* Check for a conflict in the node */
	bool found_conflict = node->get_conflicts(&agent_1,conflict_1,&agent_2,conflict_2);

	/* Make sure a conflict is found */
	if (found_conflict == false)
	{
		std::cout << "FAILED: No CBSNode conflict found." << std::endl;
		Tests::cbs_node_cleanup(a_1, a_2, node);
		return false;
	}

	/* Make sure the conflict found is correct */
	if (
		conflict_1->get_coord()->get_xcoord() != 1 ||
		conflict_1->get_coord()->get_ycoord() != 0 ||
		conflict_2->get_coord()->get_xcoord() != 1 ||
		conflict_2->get_coord()->get_ycoord() != 0 ||
		conflict_1->get_depth() != 1 || conflict_2->get_depth() != 1
		)
	{
		std::cout << "FAILED: Incorrect CBSNode conflict." << std::endl;
		Tests::cbs_node_cleanup(a_1, a_2, node);
		return false;
	}

	/* Create a new CBSNode based on the prior CBSNode with a new conflict */
	CBSNode* conflict_node = new CBSNode(node, agent_1, conflict_1);

	/* Check for a conflict in the node */
	found_conflict = conflict_node->get_conflicts(&agent_1, conflict_1, &agent_2, conflict_2);

	/* Make sure a conflict is found */
	if (found_conflict == false)
	{
		std::cout << "FAILED: No second CBSNode conflict found." << std::endl;
		Tests::cbs_node_cleanup(a_1, a_2, node);
		delete conflict_node;
		return false;
	}

	/* Make sure the conflict found is correct */
	if (
		conflict_1->get_coord()->get_xcoord() != 1 ||
		conflict_1->get_coord()->get_ycoord() != 0 ||
		conflict_2->get_coord()->get_xcoord() != 2 ||
		conflict_2->get_coord()->get_ycoord() != 0 ||
		conflict_1->get_depth() != 2 || conflict_2->get_depth() != 2
		)
	{
		std::cout << "FAILED: Incorrect second CBSNode conflict." << std::endl;
		Tests::cbs_node_cleanup(a_1, a_2, node);
		delete conflict_node;
		return false;
	}

	/* Clean up dynamic memory */
	Tests::cbs_node_cleanup(a_1, a_2, node);
	delete conflict_node;
	delete conflict_1;
	delete conflict_2;

	/* All tests passed */
	return true;
}

/*
* Cleanup cbs_node_test function
* @param a_1: Pointer to delete
* @param a_2: Pointer to delete
* @param node: Pointer to delete
*/
void Tests::cbs_node_cleanup(Agent* a_1, Agent* a_2, CBSNode* node)
{
	delete a_1;
	delete a_2;
	delete node;
}

/*
* CBSTree Functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::cbs_tree_tests()
{
	/* Create world file */
	std::string world_file = create_world_file();

	/* Create agent file */
	std::string agent_file = create_agent_file();

	/* Create the CBSTree */
	CBSTree* tree = new CBSTree(agent_file, world_file);

	/* Get the solution */
	CBSNode* solution_node = tree->get_solution();

	/* Get the solution agents */
	std::vector<Agent*> solution_agents = *solution_node->get_agents();

	/* Get the solutions for each agent */
	std::stack<Coord> sol_0 = solution_agents[0]->get_solution();
	std::stack<Coord> sol_1 = solution_agents[1]->get_solution();
	std::stack<Coord> sol_2 = solution_agents[2]->get_solution();

	/* Check the solution path for the first agent */
	Coord* c0 = new Coord(0, 0);
	Coord* c1 = new Coord(1, 1);
	Coord* c2 = new Coord(2, 2);
	if (Tests::check_top_coord(sol_0, c0) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_0, c1) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_0, c2) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}

	/* Clean up */
	Tests::cbs_tree_cleanup(c0, c1, c2);

	/* Check the solution path for the second agent */
	c0 = new Coord(1, 0);
	c1 = new Coord(2, 1);
	c2 = new Coord(1, 2);
	if (Tests::check_top_coord(sol_1, c0) == false)
	{
		cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_1, c1) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_1, c2) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}

	/* Clean up */
	Tests::cbs_tree_cleanup(c0, c1, c2);

	/* Check the solution path for the third agent */
	c0 = new Coord(2, 2);
	c1 = new Coord(1, 2);
	c2 = new Coord(0, 1);
	if (Tests::check_top_coord(sol_2, c0) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_2, c1) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}
	if (Tests::check_top_coord(sol_2, c2) == false)
	{
		Tests::cbs_tree_cleanup(c0, c1, c2);
		return false;
	}

	/* Clean up */
	Tests::cbs_tree_cleanup(c0, c1, c2);

	/* Remove the test files */
	std::remove(world_file.c_str());
	std::remove(agent_file.c_str());

	/* All tests passed */
	return true;
}

/*
* Cleanup cbs_tree_test function
* @param c0: Pointer to delete
* @param c1: Pointer to delete
* @param c2: Pointer to delete
*/
void Tests::cbs_tree_cleanup(Coord* c0, Coord* c1, Coord* c2)
{
	delete c0;
	delete c1;
	delete c2;
}

/*
* Create a 3x3 world with no obstacles
* @return a string file name for the newly created world file
*/
std::string Tests::create_world_file()
{
	/* Name of the world file */
	char test_file[] = "Worlds/test_file.txt";

	/* Create the world */
	std::ofstream file(test_file);
	file << "111\n";
	file << "111\n";
	file << "111\n";
	file.close();

	return std::string(test_file);
}

/*
* Create a file containing several agents 
* @return a string file name for the newly created agent file
*/
std::string Tests::create_agent_file()
{
	/* Name of the agent file */
	char test_file[] = "Agents/agent_file.txt";

	/* Create 3 agents */
	std::ofstream file(test_file);
	file << "Agent_1 (0,0) (2,2)\n";
	file << "Agent_2 (1,0) (1,2)\n";
	file << "Agent_3 (2,2) (0,1)\n";
	file.close();

	return std::string(test_file);
}

/* 
* Check that the next coordinate in the path is correct
* @param path: The path to check
* @param check_coord: The coordinate the next path coordinate must be equal to
* @return true if the next path coordinate equals check_coord, false otherwise
*/
bool Tests::check_top_coord(std::stack<Coord>& path, Coord* check_coord)
{
	/* Make sure there is another element in the path and that it is correct */
	if (path.size() == 0 || path.top() != *check_coord)
	{
		std::cout << "FAILED: CBSTree solution not correct." << std::endl;
		return false;
	}

	/* Move on to the next coordinate in the path */
	path.pop();

	return true;
}