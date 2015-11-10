#ifndef TESTS_H
#define TESTS_H

#include <string>
#include <stack>

class Coord;
class World;
class Agent;
class CBSNode;

class Tests
{
public:
	/* Runs all tests */
	static bool run_tests();

	/* Tests for each object */
	static bool coordinate_tests();
	static bool position_tests();
	static bool cantor_pair_tests();
	static bool closed_list_tests();
	static bool world_tests();
	static bool a_star_tests();
	static bool path_clear_a_star_tests();
	static bool cbs_node_tests();
	static bool cbs_tree_tests();
	
	/* Non-automated tests */
	static void print_world_test(std::string test_file);
	
	/* Auxiliary functions for tests */
	static bool coordinate_equality(Coord* coord_1, Coord* coord_2);
	static World* create_world();
	static std::string create_world_file();
	static std::string create_agent_file();
	static bool check_top_coord(std::stack<Coord>& path, Coord* check_coord);
	static void cbs_node_cleanup(Agent* a_1, Agent* a_2, CBSNode* node);
	static void cbs_tree_cleanup(Coord* c0, Coord* c1, Coord* c2);
};

#endif