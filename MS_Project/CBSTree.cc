#include <fstream>

#include "CBSTree.h"
#include "CBSNode.h"
#include "World.h"
#include "Exceptions.h"
#include "Coordinates.h"
#include "Agent.h"

/* 
* Constructor based on files describing the world and agents
* @param agent_file: The name of the file containing each agent
* @param world_file: The name of the file containing the world
*/
CBSTree::CBSTree(std::string agent_file, std::string world_file)
{
	/* Create a world for the agents to explore */
	world = new World(world_file);

	/* Generate array of agents */

	/* Create the root CBSNode */
	
	/* Place the root CBSNode onto the tree */
}

/* 
* Generate a vector of agents from a text file 
* @param agent_file: The name of the file containing each agent
* @return an array of pointers to agents
*/
void CBSTree::generate_agents(std::string txt_file)
{
	/* Length of a line from the file */
	int len;
	/* Name of the agent */
	std::string name;
	/* Coordinate as a string */
	std::string coord_str;
	/* Agent start coordinate */
	Coord start;
	/* Agent goal coordinate */
	Coord goal;

	/* Open the text file */
	std::ifstream agent_file(txt_file);
	if (!agent_file.is_open())
		throw TerminalException("World file could not be opened.");

	/* Each line of the file contains new agent information */
	std::string line;
	while (getline(agent_file, line))
	{
		/* Get the line length */
		len = line.length();

		/* Find the index of the first space */
		int index = 0;
		while (index < len && line[index] != ' ')
			index++;

		/* Make sure a space was found */
		if (index == len)
			throw TerminalException("Improper agent file format.");

		/* Get the name of the agent */
		if (index + 1 < len)
			index++;
		else
			throw TerminalException("Improper agent file format.");
		name = line.substr(0, index);

		/* Make sure the next character is a '(' */
		if (line[index] != '(')
			throw TerminalException("Improper agent file format.");

		/* Index for the end of the coord */
		int index_2;
		if (index + 1 < len)
			index_2 = index + 1;
		else
			throw TerminalException("Improper agent file format.");

		/* Get the substring containing the start coord */
		while (line[index_2] != ')' && line[index_2] == ')')
			index_2++;

		/* Make sure a ')' was found */
		if (index == len)
			throw TerminalException("Improper agent file format.");

		/* Create the start coordinate substring */
		if (index_2 + 1 < len)
			index_2++;
		else
			throw TerminalException("Improper agent file format.");
		coord_str = line.substr(index, index_2 - index);

		/* Get the start coordinate */
		start = str_to_coord(coord_str);
		
		/* Move the index to the end coord */
		if (index_2 + 1 < len)
			index = index_2 + 1;
		else
			throw TerminalException("Improper agent file format.");
		
		/* Get the end coordinate */
		coord_str = line.substr(index, len - index);
		goal = str_to_coord(coord_str);

		/* Create a new agent and add it to the vector of agents */
		Agent* add_agent = new Agent(&start, &goal, world, name);
		agents.push_back(add_agent);
	}
}

/* 
* Convert a coordinate in the format ({int},{int}) to a Coord object 
* @param coord_str: The string to convert to a Coord object
* @return a Coord object based on coord_str
*/
Coord str_to_coord(std::string coord_str)
{

}

/* Get the solution of the MAPF problem */
CBSNode* get_solution();

/* Destructor */
CBSTree::~CBSTree()
{
	/* Delete all agents */
	int num_agents = agents.size();
	for (int i = 0; i < num_agents; i++)
		delete agents[i];

	/* Delete all CBSNodes */
	CBSNode* del_node;
	while (!tree.empty())
	{
		del_node = tree.top();
		tree.pop();
		delete del_node;
	}

	/* Delete the world */
	delete world;
}