#include <fstream>

#include "CBSTree.h"
#include "CBSNode.h"
#include "World.h"
#include "Exceptions.h"
#include "Coordinates.h"
#include "Agent.h"
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
CBSTree::CBSTree(std::string agent_file, std::string world_file)
{
	/* Create a world for the agents to explore */
	world = new World(world_file);

	/* Generate array of agents */
	generate_agents(agent_file);

	/* Create the root CBSNode */
	CBSNode* root = new CBSNode(&agents);

	/* Place the root CBSNode onto the tree */
	tree.push(root);

	/* Initialize closed CBSNode list */
	closed_nodes = std::vector<CBSNode*>();

#ifdef TIME_LIMIT
	start = time(0);
#endif
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
		throw TerminalException("Agent file could not be opened.");

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
			throw TerminalException("Improper agent file format.");
		name = line.substr(0, index - 1);

		/* Index for the end of the coord */
		int index_2;
		if (index + 1 < len)
			index_2 = index + 1;
		else
			throw TerminalException("Improper agent file format.");

		/* Get the substring containing the start coord */
		while (line[index_2] != ')' && index_2 < len)
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
		Agent* add_agent = new Agent(start, goal, world, name);
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

	/* Make sure the first charcter is a '(' and the last character is a ')'*/
	if (coord_str[0] != '(' || coord_str[len - 1] != ')')
		throw TerminalException("Improper agent file format.");

	/* Index for traversing the string's elements */
	int index = 1;

	/* Get the x coordinate*/
	int x_coord = 0;
	int add_val;
	while (index < len && coord_str[index] != ',')
	{
		x_coord *= 10;
		add_val = coord_str[index] - '0';

		/* Make sure the character is a digit between 0 and 9, inclusive */
		if (add_val < 0 || add_val > 9)
			throw TerminalException("Improper agent file format.");

		x_coord += add_val;
		index++;
	}

	/* Make sure there is another coordinate */
	if (index >= len - 1)
		throw TerminalException("Improper agent file format.");
	
	/* Get the y coordinate */
	index++;
	int y_coord = 0;
	while (index < len && coord_str[index] != ')')
	{
		y_coord *= 10;
		add_val = coord_str[index] - '0';

		/* Make sure the character is a digit between 0 and 9, inclusive */
		if (add_val < 0 || add_val > 9)
			throw TerminalException("Improper agent file format.");

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
	while (true)
	{

/* Stop the program early if testing for time */
#ifdef TIME_LIMIT
		if (difftime(time(0), start) > TIME_LIMIT)
			return NULL;
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

		/* No solution was found, create two new nodes to add to the heap */
		CBSNode* add_node_1 = new CBSNode(top, agent_1, conflict_1);
		CBSNode* add_node_2 = new CBSNode(top, agent_2, conflict_2);
		tree.push(add_node_1);
		tree.push(add_node_2);

		/* Conflict pointers no longer necessary (copied by value when creating new CBSNodes) */
		delete conflict_1;
		delete conflict_2;

		/* Store the explored CBS Node on the closed list */
		closed_nodes.push_back(top);
	}
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
		return;

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
	int num_closed_nodes = closed_nodes.size();
	for (int i = 0; i < num_closed_nodes; i++)
		delete closed_nodes[i];

	/* Delete the world */
	delete world;
}