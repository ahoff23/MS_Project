#ifndef TESTS_H
#define TESTS_H

class Coord;

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
	
	/* Non-automated tests */
	static void print_world_test(std::string test_file);
	
	/* Auxiliary functions for tests */
	static bool coordinate_equality(Coord* coord_1, Coord* coord_2);
};

#endif