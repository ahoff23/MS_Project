#include <random>
#include <time.h>
#include <fstream>
#include <string>
#include <unordered_map>

#include "TestGenerator.h"
#include "Coordinates.h"
#include "Utils.h"
#include "AStarNodeMultiMap.h"
#include "HashStruct.h"

/*
* Constructor: Set the number of tests to generate 
* @param p_test_count: The number of tests to generate
* @param num_rows: The number of rows to generate in each world file
* @param num_cols: The number of columns to generate in each world file
*/
TestGenerator::TestGenerator(
	int p_test_count, unsigned short p_num_rows, unsigned short p_num_cols, unsigned short p_num_agents
	)
{
	/* Seed the random number generator */
	srand(time(NULL));

	/* Set the number of tests to generate */
	test_count = p_test_count;

	/* Set the number of rows and colums*/
	num_rows = p_num_rows;
	num_cols = p_num_cols;

	/* Set the number of agents per test */
	num_agents = p_num_agents;
}

/* 
* Generate world files 
* @param obs_prob: Probability that an individual coordinate is an obstacle
*/
void TestGenerator::generate_files(double obs_prob)
{
	/* Suffix of each test file */
	std::string suffix = ".txt";

	/*
	* Boolean vector where elements are true if the corresponding 
	* coordinate is free, false if it is occupied by an obstacle 
	*/
	std::vector<bool> obs_coords = std::vector<bool>();

	/* Create a world file for each test */
	for (int i = 0; i < test_count; i++)
	{
		/* Name of the file */
		std::string test_file = "TestWorlds/test_file";
		test_file += Utils::to_string(i);
		test_file += suffix;

		/* Create and open a new file */
		std::ofstream out_file(test_file);
		
		/* Row to write to the file */
		std::string write_row;

		/* num_rows determines the number of rows to write */
		for (int j = 0; j < num_rows; j++)
		{
			/* num_cols determines the number of elements in each row */
			for (int k = 0; k < num_cols; k++)
			{
				/* Determine if the next element is an obstacle or open space */
				double obs_val = static_cast<double>(rand() % 10 + 1) / 10;

				/* Write the element to the row */
				if (obs_val < obs_prob)
				{
					write_row.append("0");
					obs_coords.push_back(false);
				}
				else
				{
					write_row.append("1");
					obs_coords.push_back(true);
				}
			}
			/* Write the row to the file */
			out_file << write_row;
			out_file << "\r\n";

			/* Clear the row string */
			write_row = "";
		}

		/* Generate agent file for this test */
		generate_agents(i, &obs_coords);

		/* Clear test specific data structures */
		obs_coords.clear();

		/* Close the file */
		out_file.close();
	}
}

/*
* Generate agent files 
* @param num_test: The test number for this test
* @param obs_coords: Vector of coordinates blocked by obstacles in this test
*/
void TestGenerator::generate_agents(int test_num, std::vector<bool>* obs_coords)
{
	/* Prefix for agent names */
	std::string agent_name_prefix = "Agent_";

	/* Start coordinates already occupied */
	std::unordered_map<unsigned int, bool> starts = 
		std::unordered_map<unsigned int, bool>();

	/* Goal coordinates already occupied */
	std::unordered_map<unsigned int, bool> goals =
		std::unordered_map<unsigned int, bool>();

	/* Name of the file */
	std::string suffix = ".txt";
	std::string test_file = "TestAgents/test_file";
	test_file += Utils::to_string(test_num);
	test_file += suffix;

	/* Create and open a new file */
	std::ofstream out_file(test_file);

	/* Generate agents */
	for (int i = 0; i < num_agents; i++)
	{
		/* Add the agent's name */
		out_file << agent_name_prefix;
		out_file << Utils::to_string(i);
		out_file << " ";

		/* Start and goal coordinates for this agent */
		Coord* start_coord;
		Coord* goal_coord;

		/* Generate the start and end coordinates. They cannot be the same. */
		while (true)
		{
			start_coord = gen_coord(&starts, obs_coords);
			goal_coord = gen_coord(&goals, obs_coords);

			if (*start_coord != *goal_coord)
				break;
			else
			{
				/* Coords do not work (start can't equal goal), clear them and try again */
				delete start_coord;
				delete goal_coord;
			}
		}

		/* Add the start and goal coordinate to their respective hash tables */
		starts.emplace(HashStruct::hash_coord(start_coord), true);
		starts.emplace(HashStruct::hash_coord(goal_coord), true);

		/* Write the start and goal coordinates to the file */
		out_file << *start_coord;
		out_file << " ";
		out_file << *goal_coord;
		out_file << "\r\n";

		/* Clean up */
		delete start_coord;
		delete goal_coord;
	}
	/* Close the file */
	out_file.close();
}

/*
* Generate a coordinate 
* @param blocked_coords: hash table of coordinates occupied by another agent
* @param obs_coords: Coordinates containing an obstacle
* @return a coordinate that is an empty space and is not occupied by another agent
*/
Coord* TestGenerator::gen_coord(
	std::unordered_map<unsigned int, bool>* blocked_coords,
	std::vector<bool>* obs_coords
	)
{
	/* X and Y coordinates*/
	int x_coord;
	int y_coord;

	/* Coordinate to return */
	Coord* coord = new Coord(1,1);

	/* Repeat until a coord is generated */
	while (true)
	{
		/* Generate a possible coordinate */
		x_coord = rand() % num_cols;
		y_coord = rand() % num_rows;
		coord->set_x(x_coord);
		coord->set_y(y_coord);

		/* Make sure the coordinate is not blocked by an obstacle */
		if ((*obs_coords)[y_coord * num_cols + x_coord] == false)
			continue;

		/* Make sure the coordinate is not occupied by another agent */
		if (blocked_coords->find(HashStruct::hash_coord(coord)) == blocked_coords->end())
			return coord;
	}
}