#include <random>
#include <time.h>
#include <fstream>
#include <string>
#include <unordered_map>

#include "TestGenerator.h"
#include "Coordinates.h"
#include "UnorderedMap.h"

/*
* Constructor: Set the number of tests to generate 
* @param p_test_count: The number of tests to generate
* @param num_rows: The number of rows to generate in each world file
* @param num_cols: The number of columns to generate in each world file
*/
TestGenerator::TestGenerator(int p_test_count, int p_num_rows, int p_num_cols, int p_num_agents)
{
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
void TestGenerator::generate_files(float obs_prob)
{
	/* Suffix of each test file */
	std::string suffix = ".txt";

	/* Seed the random number generator */
	srand(time(NULL));

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
		test_file += std::to_string(i);
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
				float obs_val = float(rand() % 10 + 1) / 10;

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
			out_file << "\n";

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

	/* Seed the random number generator */
	srand(time(NULL));

	/* Start coordinates already occupied */
	std::unordered_map<int, bool> starts = std::unordered_map<int, bool>();

	/* Goal coordinates already occupied */
	std::unordered_map<int, bool> goals = std::unordered_map<int, bool>();

	/* Name of the file */
	std::string suffix = ".txt";
	std::string test_file = "TestAgents/test_file";
	test_file += std::to_string(test_num);
	test_file += suffix;

	/* Create and open a new file */
	std::ofstream out_file(test_file);

	/* Generate agents */
	for (int i = 0; i < num_agents; i++)
	{
		/* Add the agent's name */
		out_file << agent_name_prefix;
		out_file << std::to_string(i);
		out_file << " ";

		/* Start and goal coordinates for this agent */
		Coord start_coord;
		Coord goal_coord;

		/* Generate the start and end coordinates. They cannot be the same. */
		while (true)
		{
			start_coord = gen_coord(&starts, obs_coords);
			goal_coord = gen_coord(&goals, obs_coords);

			if (start_coord != goal_coord)
				break;
		}

		/* Add the start and goal coordinate to their respective hash tables */
		int start_hash = CantorPair::get_int(&start_coord);
		starts.emplace(start_hash, true);
		int goal_hash = CantorPair::get_int(&goal_coord);
		starts.emplace(goal_hash, true);

		/* Write the start and goal coordinates to the file */
		out_file << start_coord;
		out_file << " ";
		out_file << goal_coord;
		out_file << "\n";
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
Coord TestGenerator::gen_coord(
	std::unordered_map<int, bool>* blocked_coords, std::vector<bool>* obs_coords
	)
{
	/* X and Y coordinates*/
	int x_coord;
	int y_coord;

	/* Coordinate to return */
	Coord coord;

	/* Repeat until a coord is generated */
	while (true)
	{
		/* Generate a possible coordinate */
		x_coord = rand() % num_cols;
		y_coord = rand() % num_rows;
		coord = Coord(x_coord, y_coord);

		/* Make sure the coordinate is not blocked by an obstacle */
		if ((*obs_coords)[y_coord * num_cols + x_coord] == false)
			continue;

		/* Make sure the coordinate is not occupied by another agent */
		int hash = CantorPair::get_int(&coord);
		if (blocked_coords->find(hash) == blocked_coords->end())
			return coord;
	}
}