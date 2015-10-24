#include <iostream>
#include <ctime>

#include "Tests.h"
#include "CBSTree.h"
#include "Exceptions.h"
#include "Macros.h"





#include <queue>
#include <random>
#include <time.h>
int main()
{
	std::clock_t start;
	srand(time(NULL));

	// Create priority queue
	std::priority_queue<int> test = std::priority_queue<int>();

	// Add elements to queue
	const int ADD_AMT = 100000;
	for (int i = 0; i < ADD_AMT; i++)
		test.push(rand() % 100);

	// Get start time
	start = std::clock();

	// Add 100 elements to queue
	const int TEST_ADD_AMT = 100;
	for (int i = 0; i < TEST_ADD_AMT; i++)
		test.push(rand() % 100);

	// Print out duration of adding elements
	std::cout << "TEST TIME: " << float(std::clock() - start) / CLOCKS_PER_SEC << std::endl;

	return 0;
}





int test()
{
#ifndef TEST
	/* Agent and world file names */
	std::string agent_file = "Agents/testAgents8.txt";
	std::string world_file = "Worlds/testWorld8.txt";

	/* Tree variable */
	CBSTree* tree;

	/* Create the CBSTree and output its solution to an output file */
	try
	{
#ifdef TIME_TEST
		/* Get the start time of the test */
		std::clock_t start;
		start = std::clock();
#endif

		tree = new CBSTree(agent_file, world_file);
		tree->file_print_solution();

#ifdef TIME_TEST
		/* Print the duration of the test in seconds */
		std::cout << "TEST TIME: " << float(std::clock() - start) / CLOCKS_PER_SEC << std::endl;
#endif
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
#else
	Tests::run_tests();
#endif

	return 0;
}