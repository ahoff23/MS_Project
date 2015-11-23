#include <iostream>
#include <ctime>
#include <fstream>
#include <math.h>

#include "Tests.h"
#include "CBSTree.h"
#include "Exceptions.h"
#include "Macros.h"
#include "Utils.h"
#include "CBSNode.h"


void catch_failure(
	int& failures, int &depth_exceeded, std::string exception_msg, 
	std::ofstream* output_file, int test_num
	);
double test_stats(
	std::vector<double>* test_times, std::ofstream* output_file, int test_num
	);

#ifdef NUM_GEN_TESTS
#include "TestGenerator.h"
#endif

int main()
{
#ifdef NUM_GEN_TESTS
	/* Number of probability of an element being an obstacle */
	const double OBS_PROB = 0.35;

	/* Number of rows */
	const int NUM_ROWS = 20;

	/* Number of columns */
	const int NUM_COLS = 6;

	/* Number of agents to generate */
	const int AGENTS_PER_FILE = 25;

	TestGenerator test_gen = TestGenerator(NUM_GEN_TESTS,NUM_ROWS,NUM_COLS,AGENTS_PER_FILE);
	test_gen.generate_files(OBS_PROB);
#endif

#ifdef RUN_PROGRAM
	/* Agent and world file names */
	std::string agent_file = "Agents/testAgents6.txt";
	std::string world_file = "Worlds/testWorld6.txt";

	/* Tree variable */
	CBSTree* tree;

	/* Create the CBSTree and output its solution to an output file */
	try
	{
		tree = new CBSTree(agent_file, world_file);
		tree->file_print_solution();
		delete tree;
	}
	catch (TerminalException& ex)
	{
		/* Get the exception message */
		std::string exception_msg = ex.what();

		/* Special Case: Time limit exceeded exception */
		if (exception_msg == "TIME LIMIT EXCEEDED")
		{
			std::cout << "Test failed." << std::endl;
			std::cin.get();
		}
		/* Print the error message */
		else
			std::cout << exception_msg << std::endl;
	}
#endif

#ifdef TEST
	Tests::run_tests();
#endif

#ifdef NUM_TESTS
	/* Start time of the test */
	std::clock_t start;
	std::clock_t pca_start;

	/* Tree variable */
	CBSTree* tree = NULL;

	/* Variable storing the sum of all test times */
	double sum_time = 0;

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

	/* Test cost */
	int cost;

	/* True if a test in this series of tests failed, false otherwise */
	bool test_failed = false;

	/* Loop through each test */
	for (int i = 0; i < NUM_TESTS; i++)
	{
		/* Create the world and agent file */
		std::string world_file = world_file_prefix + Utils::to_string(i) + suffix;
		std::string agent_file = agent_file_prefix + Utils::to_string(i) + suffix;

		/* Print the test number to the console */
		std::cout << "Currently running test number " << i << "." << std::endl;

		/* No test failed yet since none were run */
		test_failed = false;

		/* Vector containing each test time */
		std::vector<double> test_times;

		for (int j = 0; j < TEST_RUN_COUNT; j++)
		{
			/* Don't run again if one test failed */
			if (test_failed == true)
				break;

			/* Start the timer */
			start = std::clock();

			/* Create the CBSTree and find the solution */
			try
			{
				tree = new CBSTree(agent_file, world_file);

#ifdef CONFLICT_CORRECTION_TIME
				pca_start = std::clock();
#endif

				CBSNode* sol = tree->get_solution();
				cost = sol->get_cost();
				delete sol;
				delete tree;
			}
			catch (TerminalException& ex)
			{
				catch_failure(failures, depth_exceeded, ex.what(), &output_file, i);
				test_failed = true;
				continue;
			}

			/* Store the test time */
#ifdef CONFLICT_CORRECTION_TIME
			test_times.push_back(double(std::clock() - pca_start) / CLOCKS_PER_SEC);
#else
			test_times.push_back(double(std::clock() - start) / CLOCKS_PER_SEC);
#endif
		}
		if (test_failed == false)
			sum_time += test_stats(&test_times, &output_file, i);
	}

	/* Print the duration of the test in seconds */
	output_file << "AVG TEST TIME: " << sum_time / (NUM_TESTS - failures) << " seconds.\r\n";
	output_file << failures << " failures out of " << NUM_TESTS << " total tests.\r\n";
	output_file << depth_exceeded << " tests exceeded the max search depth out of " << 
		NUM_TESTS << " total tests.\r\n";
	output_file.close();
#endif

	return 0;
}

/*
* Deal with an exception
* @param failures: The number of failures that have occurred so far
* @param depth_exceeded: The number of tests in which an agent has exceeded the max depth so far
* @param exception_msg: The message from the exception that caused the failure
* @param output_file: The file to output results to
* @param test_num: The number of the test being run
*/
void catch_failure(
	int& failures, int &depth_exceeded, std::string exception_msg, 
	std::ofstream* output_file, int test_num
	)
{
	/* Special Case: Time limit exceeded exception */
	if (exception_msg == "TIME LIMIT EXCEEDED")
	{
		std::cout << "Test " << test_num << " failed." << std::endl;
		*output_file << "Test " << test_num << " failed. \r\n";
		failures++;
	}
	else if (exception_msg == "Exceeded max search depth.")
	{
		/* An agent exceeded max search depth */
		std::cout << "An agent in test " << test_num << 
			" exceeded the max search depth." << std::endl;
		*output_file << "An agent in test " << test_num << 
			" exceeded the max search depth.\r\n";
		depth_exceeded++;
	}
	else if (exception_msg == "Ran out of CBS nodes.")
	{
		std::cout << "An agent in test " << test_num << 
			" ran out of CBS nodes to expand." << std::endl;
		*output_file << "An agent in test " << test_num << 
			" ran out of CBS nodes to expand.\r\n";
		failures++;
	}
	/* Print the error message */
	else
		std::cout << exception_msg << std::endl;
}

/*
* Print out statistics for each test
* @param test_times: Vector containing the completion times for each test
* (may or may not include the first A* search for each agent)
* @param output_file: The file to output results to
* @param test_num: The number of the test being run
* @return the average of all test times
*/
double test_stats(std::vector<double>* test_times, std::ofstream* output_file, int test_num)
{
	/* Get the mean */
	double mean = 0;
	for (int i = 0; i < test_times->size(); i++)
		mean += (*test_times)[i];
	mean /= test_times->size();

	/* Get the standard deviation */
	double mean_sq = 0;
	for (int i = 0; i < test_times->size(); i++)
		mean_sq += ((*test_times)[i] - mean) * ((*test_times)[i] - mean);
	mean_sq /= (*test_times).size();
	double sdev = sqrt(mean_sq);

	/* Get a 95% confidence interval */
	double ci_low = mean - (1.96 * (sdev / sqrt(test_times->size())));
	double ci_high = mean + (1.96 * (sdev / sqrt(test_times->size())));

	/* Print results */
	*output_file << "Test " << test_num << " took " << mean << " seconds to complete "
#ifdef CONFLICT_CORRECTION_TIME
		"(excluding the first A*) " <<
#endif
		"with a standard deviation of " << sdev <<
		" and a 95% confidence interval of [" << ci_low << "," <<
		ci_high << "].\r\n";

	return mean;
}