#include <iostream>

#include "Tests.h"
#include "CBSTree.h"
#include "Exceptions.h"

#define TEST 1

int main()
{
#ifndef TEST
	/* Agent and world file names */
	std::string agent_file = "Agents/testAgents7.txt";
	std::string world_file = "Worlds/testWorld7.txt";

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
#else
	Tests::run_tests();
#endif

	return 0;
}