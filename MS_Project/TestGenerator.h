#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <unordered_map>

struct hash_struct;

class Coord;

class TestGenerator
{
public:
	/* Constructor: Set the number of tests to generate */
	TestGenerator(int p_test_count, int p_num_rows, int p_num_cols, int p_num_agents);
	/* Genearte world files */
	void generate_files(float obs_prob);
private:
	/* Number of tests to generate */
	int test_count;
	/* Number of rows */
	int num_rows;
	/* Number of columns */
	int num_cols;
	/* Number of agents in each test */
	int num_agents;

	/* Generate agent files */
	void generate_agents(int test_num, std::vector<bool>* obs_coords);
	/* Generate a coordinate */
	Coord gen_coord(
		std::unordered_map<int, bool>* blocked_coords, std::vector<bool>* obs_coords
		);
};

#endif