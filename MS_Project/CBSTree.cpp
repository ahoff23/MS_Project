#include <fstream>

#include "CBSTree.h"
#include "CBSNode.h"
#include "World.h"
#include "Exceptions.h"
#include "Coordinates.h"
#include "Agent.h"
#include "Macros.h"

#ifdef CONFLICT_DATA
#include <iostream>
#endif

#ifdef TIME_LIMIT
#include <ctime>
#endif

/* 
* Operator for comparing two CBSNodes in the Compare struct for use in priority queue (minheap)
* @param lhs: The first CBSNode to compare
* @param rhs: The second CBSNode to compare
* @return true if lhs' cost is less than rhs' cost, false otherwise
*/
bool Compare::operator()(const CBSNode* lhs, const CBSNode* rhs) const
{
	return lhs->get_cost() > rhs->get_cost();
}

/*
* Constructor based on files describing the world and agents
* @param agent_file: The name of the file containing each agent
* @param world_file: The name of the file containing the world
*/
#include <iostream>
CBSTree::CBSTree(std::string agent_file, std::string world_file)
{
#ifdef TIME_LIMIT
	/* Get the start time of the algorithm */
	start_time = std::clock();
#endif

	/* Initialize closed CBSNode list */
	closed_nodes = std::vector<CBSNode*>();

	/* Create a world for the agents to explore */
	world = new World(world_file);

	/* Generate array of agents */
	generate_agents(agent_file);

	/* Create the root CBSNode */
	CBSNode* root = new CBSNode(&agents);

	/* Place the root CBSNode onto the tree */
	tree.push(root);
}

/* 
* Generate a vector of agents from a text file 
* @param agent_file: The name of the file containing each agent
* @return an array of pointers to agents
*/
void CBSTree::generate_agents(std::string txt_file)
{
	/* ASCII value for carriage return */
	const int CARRIAGE_RETURN = 13;

	/* Length of a line from the file */
	int len;
	/* Name of the agent */
	std::string name;
	/* Coordinate as a string */
	std::string coord_str;
	/* Agent start coordinate */
	Coord* start;
	/* Agent goal coordinate */
	Coord* goal;

	/* Open the text file */
	std::ifstream agent_file(txt_file);
	if (!agent_file.is_open())
		throw TerminalException("AGENT ERROR 1: Agent file could not be opened.");

	/* Each line of the file contains new agent information */
	std::string line;
	while (getline(agent_file, line))
	{
		/* Make sure the line isn't commented out */
		if (line[0] == '#')
			continue;

		/* Get the line length */
		len = line.length();

		/* If the last character is a carriage return, ignore it */
		if (line.at(len - 1) == CARRIAGE_RETURN)
			len--;

		/* Find the index of the first space */
		int index = 0;
		while (index < len && line[index] != ' ')
			index++;

		/* Get the name of the agent */
		if (index + 1 < len)
			index++;
		else
			throw TerminalException("AGENT ERROR 2: Improper agent file format.");
		name = line.substr(0, index - 1);

		/* Index for the end of the coord */
		int index_2;
		if (index + 1 < len)
			index_2 = index + 1;
		else
			throw TerminalException("AGENT ERROR 3: Improper agent file format.");

		/* Get the substring containing the start coord */
		while (line[index_2] != ')' && index_2 < len)
			index_2++;

		/* Make sure a ')' was found */
		if (index == len)
			throw TerminalException("AGENT ERROR 4: Improper agent file format.");

		/* Create the start coordinate substring */
		if (index_2 + 1 < len)
			index_2++;
		else
			throw TerminalException("AGENT ERROR 5: Improper agent file format.");
		coord_str = line.substr(index, index_2 - index);

		/* Get the start coordinate */
		start = str_to_coord(coord_str);
		
		/* Move the index to the end coord */
		if (index_2 + 1 < len)
			index = index_2 + 1;
		else
			throw TerminalException("AGENT ERROR 6: Improper agent file format.");

		/* Get the end coordinate */
		coord_str = line.substr(index, len - index);
		goal = str_to_coord(coord_str);

		/* Create a new agent and add it to the vector of agents */
		Agent* add_agent = new Agent(start, goal, world, name,start_time);
		agents.push_back(add_agent);

		/* Clean up start and goal */
		delete start;
		delete goal;
	}
}

/* 
* Convert a coordinate in the format ({int},{int}) to a Coord object 
* @param coord_str: The string to convert to a Coord object
* @return a Coord object based on coord_str
*/
Coord* CBSTree::str_to_coord(std::string coord_str)
{
	/* Get the string's length */
	int len = coord_str.length();

	/* ASCII value for carriage return */
	const int CARRIAGE_RETURN = 13;

	/* Ignore line breaks if one is found */
	if (coord_str[len - 1] == CARRIAGE_RETURN)
		len--;

	/* Make sure the first charcter is a '(' and the last character is a ')'*/
	if (coord_str[0] != '(' || coord_str[len - 1] != ')')
		throw TerminalException("AGENT ERROR 7: Improper agent file format.");

	/* Index for traversing the string's elements */
	int index = 1;

	/* Get the x coordinate*/
	unsigned short x_coord = 0;
	int add_val;
	while (index < len && coord_str[index] != ',')
	{
		x_coord *= 10;
		add_val = coord_str[index] - '0';

		/* Make sure the character is a digit between 0 and 9, inclusive */
		if (add_val < 0 || add_val > 9)
			throw TerminalException("AGENT ERROR 8: Improper agent file format.");

		x_coord += add_val;
		index++;
	}

	/* Make sure there is another coordinate */
	if (index >= len - 1)
		throw TerminalException("AGENT ERROR 9: Improper agent file format.");
	
	/* Get the y coordinate */
	index++;
	unsigned short y_coord = 0;
	while (index < len && coord_str[index] != ')')
	{
		y_coord *= 10;
		add_val = coord_str[index] - '0';

		/* Make sure the character is a digit between 0 and 9, inclusive */
		if (add_val < 0 || add_val > 9)
			throw TerminalException("AGENT ERROR 10: Improper agent file format.");

		y_coord += add_val;
		
		/* Add next digit */
		index++;
	}

	/* Create the coordinate and return it */
	return new Coord(x_coord, y_coord);
}

/* 
* Get the solution of the MAPF problem 
* @return the solution CBSNode
*/
CBSNode* CBSTree::get_solution()
{
	while (!tree.empty())
	{
#ifdef TIME_LIMIT
		/* Stop the program early if testing for time */
		if ((std::clock() - start_time) / CLOCKS_PER_SEC > TIME_LIMIT)
			throw TerminalException("TIME LIMIT EXCEEDED");
#endif

		/* Get the cheapest CBS Node in the heap */
		CBSNode* top = tree.top();
		tree.pop();

		/* First conflict agent's index and conflict position */
		int agent_1;
		Position* conflict_1 = new Position();

		/* Second conflict agent's index and conflict position */
		int agent_2;
		Position* conflict_2 = new Position();

		/* Get the conflicts in the node and check if a solution was found */
		if (!top->get_conflicts(&agent_1, conflict_1, &agent_2, conflict_2))
			return top;

#ifdef CONFLICT_DATA
		else
		{
			std::cout << "Conflict 1: " << *conflict_1->get_coord() << " at depth " <<
				conflict_1->get_depth() << std::endl;
			std::cout << "Conflict 2: " << *conflict_2->get_coord() << " at depth " <<
				conflict_2->get_depth() << std::endl;
		}
#endif

		/* No solution was found, create two new nodes to add to the heap */
		try
		{
			/* Only add a node if it does not run out of nodes in the A* search */
			CBSNode* add_node_1 = new CBSNode(top, agent_1, conflict_1);
			tree.push(add_node_1);
		}
		catch (OutOfNodesException& ex)
		{
			/* No need to do anything in this catch block */
			;
		}
		try
		{
			/* Only add a node if it does not run out of nodes in the A* search */
			CBSNode* add_node_2 = new CBSNode(top, agent_2, conflict_2);
			tree.push(add_node_2);
		}
		catch (OutOfNodesException& ex)
		{
			/* No need to do anything in this catch block */
			;
		}

		/* Conflict pointers no longer necessary (copied by value when creating new CBSNodes) */
		delete conflict_1;
		delete conflict_2;

		/* Store the explored CBS Node on the closed list */
		closed_nodes.push_back(top);
	}
	/* Throw an error if you run out of CBS Nodes */
	throw TerminalException("Ran out of CBS nodes.");
}

/* 
* Print the solution of the tree to an output file
*/
void CBSTree::file_print_solution()
{
	/* Get the solution */
	CBSNode* solution_node = get_solution();

	/* Make sure a solution node is returned */
	if (solution_node == NULL)
		throw TerminalException("No path found.");

	/* Get the solution agents */
	std::vector<Agent*> solution_agents = *solution_node->get_agents();

	/* Name of the output file */
	char out_file[] = "Output/solution.txt";

	/* Open the output file */
	std::ofstream file(out_file);

	/* Print each agent's solution in the output file */
	int len = solution_agents.size();
	for (int i = 0; i < len; i++)
		solution_agents[i]->file_print_solution(file);

	/* Close the output file */
	file.close();
}

/*
* Destructor 
*/
CBSTree::~CBSTree()
{
	/* Delete all agents */
	int num_agents = agents.size();
	for (int i = 0; i < num_agents; i++)
		delete agents[i];

	/* Delete all open CBSNodes */
	CBSNode* del_node;
	while (!tree.empty())
	{
		del_node = tree.top();
		tree.pop();
		delete del_node;
	}

	/* Delete all closed CBS Nodes */
	while (!closed_nodes.empty())
	{
		del_node = closed_nodes.back();
		closed_nodes.pop_back();
		delete del_node;
	}

	/* Delete the world */
	delete world;
}