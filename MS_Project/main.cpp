#include <iostream>
#include <ctime>

#include "Tests.h"
#include "CBSTree.h"
#include "Exceptions.h"
#include "Macros.h"

#ifdef NUM_GEN_TESTS
#include "TestGenerator.h"
#endif

int main()
{

#ifdef NUM_GEN_TESTS
	/* Number of probability of an element being an obstacle */
	const float OBS_PROB = 0.3;

	/* Number of rows */
	const int NUM_ROWS = 15;

	/* Number of columns */
	const int NUM_COLS = 15;

	/* Number of agents to generate */
	const int AGENTS_PER_FILE = 5;

	TestGenerator test_gen = TestGenerator(NUM_GEN_TESTS,NUM_ROWS,NUM_COLS,AGENTS_PER_FILE);
	test_gen.generate_files(OBS_PROB);
#endif

#ifdef RUN_PROGRAM
	/* Agent and world file names */
	std::string agent_file = "Agents/testAgents8.txt";
	std::string world_file = "Worlds/testWorld8.txt";

	/* Tree variable */
	CBSTree* tree;

	/* Create the CBSTree and output its solution to an output file */
	try
	{
		tree = new CBSTree(agent_file, world_file);
		tree->file_print_solution();
	}
	catch (TerminalException& ex)
	{
		/* Get the exception message */
		std::string exception_msg = ex.what();

		/* Print the error message and end the program */
		std::cout << exception_msg << std::endl;

		return 0;
	}	
	delete tree;
#endif

#ifdef TEST
	Tests::run_tests();
#endif

#ifdef NUM_TESTS
	/* Start time of the test */
	std::clock_t start;

	/* Tree variable */
	CBSTree* tree;

	/* Variable storing the sum of all test times */
	float sum_time = 0;

	/* Failure count */
	int failures;

	/* Test file name prefixes and suffixes */
	std::string world_file_prefix = "TestWorlds/test_file";
	std::string agent_file_prefix = "TestAgents/test_file";
	std::string suffix = ".txt";

	/* Loop through each test */
	for (int i = 0; i < NUM_TESTS; i++)
	{
		/* Create the world and agent file */
		std::string world_file = world_file_prefix + std::to_string(i) + suffix;
		std::string agent_file = agent_file_prefix + std::to_string(i) + suffix;

		/* Start the timer */
		start = std::clock();

		/* Create the CBSTree and find the solution */
		try
		{
			tree = new CBSTree(agent_file, world_file);
		}
		catch (TerminalException& ex)
		{
			/* Get the exception message */
			std::string exception_msg = ex.what();

			if (exception_msg == "TIME LIMIT EXCEEDED")
			{
				failures++;
				continue;
			}

			/* Print the error message and end the program */
			std::cout << exception_msg << std::endl;

			return 0;
		}
		delete tree;

		/* Get the time it took to run the test */
		sum_time += float(std::clock() - start) / CLOCKS_PER_SEC;
	}

	/* Print the duration of the test in seconds */
	std::cout << "AVG TEST TIME: " << sum_time / NUM_TESTS << " seconds." << std::endl;
#endif

	return 0;
}