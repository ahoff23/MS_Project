#include <iostream>
#include <ctime>
#include <fstream>

#include "Tests.h"
#include "CBSTree.h"
#include "Exceptions.h"
#include "Macros.h"
#include "Utils.h"
#include "CBSNode.h"

#ifdef NUM_GEN_TESTS
#include "TestGenerator.h"
#endif

int main()
{
#ifdef NUM_GEN_TESTS
	/* Number of probability of an element being an obstacle */
	const float OBS_PROB = 0.5;

	/* Number of rows */
	const int NUM_ROWS = 20;

	/* Number of columns */
	const int NUM_COLS = 20;

	/* Number of agents to generate */
	const int AGENTS_PER_FILE = 10;

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

		/* Special Case: Time limit exceeded exception */
		if (exception_msg == "TIME LIMIT EXCEEDED")
		{
			std::cout << "Test " << i << " failed." << std::endl;
			output_file << "Test " << i << " failed. \r\n";
			failures++;
		}
		/* Print the error message */
		else
			std::cout << exception_msg << std::endl;

		continue;
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
	CBSTree* tree = NULL;

	/* Variable storing the sum of all test times */
	float sum_time = 0;

	/* Failure count */
	int failures = 0;

	/* Search depth exceeded count */
	int depth_exceeded = 0;

	/* Test file name prefixes and suffixes */
	std::string world_file_prefix = "TestWorlds/test_file";
	std::string agent_file_prefix = "TestAgents/test_file";
	std::string suffix = ".txt";

	/* Open the output file */
	std::ofstream output_file("Output/avg_test_time.txt");

	/* Loop through each test */
	for (int i = 8; i < NUM_TESTS; i++)
	{
		if (i == 14)
			continue;

		/* Create the world and agent file */
		std::string world_file = world_file_prefix + Utils::to_string(i) + suffix;
		std::string agent_file = agent_file_prefix + Utils::to_string(i) + suffix;

		/* Print the test number to the console */
		std::cout << "Currently running test number " << i << "." << std::endl;

		/* Start the timer */
		start = std::clock();

		/* Create the CBSTree and find the solution */
		try
		{
			tree = new CBSTree(agent_file, world_file);
			CBSNode* sol = tree->get_solution();
			delete sol;
			delete tree;
		}
		catch (TerminalException& ex)
		{
			/* Get the exception message */
			std::string exception_msg = ex.what();

			/* Special Case: Time limit exceeded exception */
			if (exception_msg == "TIME LIMIT EXCEEDED")
			{
				std::cout << "Test " << i << " failed." << std::endl;
				output_file << "Test " << i << " failed. \r\n";
				failures++;
			}
			else if (exception_msg == "Exceeded max search depth.")
			{
				/* An agent exceeded max search depth */
				std::cout << "An agent in test " << i << " exceeded the max search depth." << std::endl;
				output_file << "An agent in test " << i << " exceeded the max search depth.\r\n";
				depth_exceeded++;
			}
			/* Print the error message */
			else
				std::cout << exception_msg << std::endl;

			continue;
		}

		/* Get the time it took to run the test */
		output_file << "Test " << i << " took " << 
			float(std::clock() - start) / CLOCKS_PER_SEC << " seconds to complete.\r\n";
		sum_time += float(std::clock() - start) / CLOCKS_PER_SEC;
	}

	/* Print the duration of the test in seconds */
	output_file << "AVG TEST TIME: " << sum_time / (NUM_TESTS - failures) << " seconds.\r\n";
	output_file << failures << " failures out of " << NUM_TESTS << " total tests." << std::endl;
	output_file << depth_exceeded << " tests exceeded the max search depth out of " << 
		NUM_TESTS << " total tests." << std::endl;
	output_file.close();
#endif
	
	for (int i = 0; i > -1; i++)
		;

	return 0;
}