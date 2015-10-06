#include <iostream>

#include "Tests.h"
#include "Coordinates.h"
#include "CantorPair.h"
#include "OpenNode.h"
#include "ClosedNode.h"
#include "ClosedList.h"

/* 
* Runs all tests 
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::run_tests()
{
	if (!coordinate_tests())
		return false;
	if (!position_tests())
		return false;
	if (!cantor_pair_tests())
		return false;
	if (!open_node_tests())
		return false;
	if (!closed_list_tests())
		return false;

	std::cout << "All tests passed." << std::endl;
	return true;
}

/*
* Tests for the Coordinate object
* @return true if all tests pass or print an error and return false if one test fails.
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
* Test CantorPair functions 
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::cantor_pair_tests()
{
	/* 
	* Test Cantor Pair function for Position object by creating Positions
	* that are similar but should all have unique Cantor Pair hashes.
	*/

	/* 3 numbers that will be rotated in ordered to slightly differentiate the Positions*/
	int num_1= 1;
	int num_2 = 2;
	int num_3 = 3;

	/* Number of positions to create */
	const int NUM_POSITIONS = 5;

	/* Create slightly different Position objects */
	Position pos_array[NUM_POSITIONS];
	pos_array[0] = Position(num_1, num_2, num_3);
	pos_array[1] = Position(num_1, num_2, num_1);
	pos_array[2] = Position(num_2, num_1, num_3);
	pos_array[3] = Position(num_3, num_2, num_1);
	pos_array[4] = Position(num_1, num_3, num_2);

	/* Create hash objects for each Position */
	int hash_array[NUM_POSITIONS];
	hash_array[0] = CantorPair::get_int(&pos_array[0]);
	hash_array[1] = CantorPair::get_int(&pos_array[1]);
	hash_array[2] = CantorPair::get_int(&pos_array[2]);
	hash_array[3] = CantorPair::get_int(&pos_array[3]);
	hash_array[4] = CantorPair::get_int(&pos_array[4]);

	/* Make sure all hashes are unique */
	for (int i = 0; i < NUM_POSITIONS; i++)
	{
		for (int j = 0; j < NUM_POSITIONS; j++)
		{
			if (hash_array[i] == hash_array[j] && i != j)
			{
				std::cout << "FAILED: Cantor pairs not unique." << std::endl;
				return false;
			}
		}
	}

	/* 
	* Test Position Cantor Pair by making sure a specifically calculated Cantor Pair hash 
	* equals the expected value.
	*/
	int expected_hash_val = (((num_1 + num_2) * (num_1 + num_2 + 1)) / 2) + num_2;
	expected_hash_val = 
		(((expected_hash_val + num_3) * (expected_hash_val + num_3 + 1)) / 2) + num_3;
	if (hash_array[0] != expected_hash_val)
	{
		std::cout << 
			"FAILED: Incorrect hash value calculated from Cantor Pair for Position object." <<
			std::endl;

		return false;
	}

	return true;
}

/*
* Test OpenNode functions
* @return true if all tests pass or print an error and return false if one test fails.
*/
bool Tests::open_node_tests()
{
	/* Test the comparison operator */
	int xcoord = 1;
	int ycoord = 2;
	int depth = 3;
	Position pos = Position(xcoord, ycoord, depth);
	OpenNode node_1 = OpenNode(nullptr, pos, depth);
	OpenNode node_2 = OpenNode(nullptr, pos, depth + 1);

	if (node_1 < node_2 || node_1 < node_1 || node_2 < node_2)
	{
		std::cout << "FAILED: OpenNode < operator failed." << std::endl;
		return false;
	}

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
	ClosedNode add_node_1 = ClosedNode(pos_1, nullptr, 1);

	/* Add a ClosedNode to the list */
	ClosedList list_1 = ClosedList();
	list_1.add_pos(&add_node_1);

	/* Check if a duplicate of the ClosedNode is in the list */
	if (!list_1.check_duplicate(&pos_1))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function did not find an existing position." <<
			std::endl;
		return false;
	}

	/* Check if a non-duplicate is not in the list */
	Position pos_2 = Position(xcoord + 1, ycoord + 1, depth);
	if (!list_1.check_duplicate(&pos_1))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function foound a non-existant duplicate." <<
			std::endl;
		return false;
	}

	/* Create a new ClosedList, with the old ClosedList as its parent */
	ClosedList list_2 = ClosedList(&list_1);

	/* Add a ClosedNode to the new list */
	ClosedNode add_node_2 = ClosedNode(pos_2, nullptr, 1);
	list_2.add_pos(&add_node_2);

	/* Check for a duplicate from the parent */	
	if (!list_1.check_duplicate(&pos_1))
	{
		std::cout <<
			"FAILED: ClosedList check_duplicate function did not find an existing position" <<
			" in a parent ClosedList." << std::endl;
		return false;
	}

	return true;
}