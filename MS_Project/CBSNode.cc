#include <unordered_map>

#include "CBSNode.h"
#include "Agent.h";
#include "Coordinates.h";
#include "CantorPair.h"

/* 
* Constructor for the first node in the CBSTree
* (i.e. not based off of a parent CBSNode)
* @param p_agents: Pointer to a vector containing pointers to each agent
*/
CBSNode::CBSNode(std::vector<Agent*>* p_agents)
{
	/* Set the list of agents to point to the parameter list of agents */
	agents = *p_agents;

	/* No agents generated by this node (they are the responsibility of the calling CBSTree */
	new_agent_num = -1;

	/* Get solution and cost for all agents	*/
	int cost = 0;
	int agent_cost;
	int num_agents = agents.size();
	for (int i = 0; i < num_agents; i++)
	{
		agent_cost = agents[i]->get_cost();
		if (agent_cost > cost)
			cost = agent_cost;
	}
}

CBSNode::CBSNode(CBSNode* parent_node, int agent_num, Position* conflict)
{
	/* Set the list of agents to point to the parent's list of agents */
	agents = *parent_node->get_agents();

	/* Create a new agent with a new conflict */
	Agent* updated_agent = new Agent(agents[agent_num], conflict);

	/* Replace the specified agent in agents with the newly created agent */
	agents[agent_num] = updated_agent;

	/* Store the index of the new agent generated in this node */
	new_agent_num = agent_num;

	/* Update the cost if this agent's cost is greater than that of the node */
	int agent_cost = agents[agent_num]->get_cost();
	if (agent_cost > cost)
		cost = agent_cost;
}

/* 
* Find one (or two) conflict positions between two agents 
* @param agent_1: Will be set to an agent num for a conflict agent
* @param conflict_1: Conflict position for agent 1
* @param agent_2: Will be set to an agent num for a conflict agent
* @param conflict_2: Conflict position for agent 2
* @return true if a conflict is found, false otherwise
*/
bool CBSNode::get_conflicts(int* agent_1, Position* conflict_1, int* agent_2, Position* conflict_2)
{
	/* Current and previous coordinates in the path */
	Coord curr_coord;
	Coord prev_coord;

	/* Hash of the current and previous coordinate */
	int hash;
	int prev_hash;

	/* Depth of the search */
	int depth = 0;

	/* Hash values if the previous and current coordinates were swapped */
	int swap_curr_hash;
	int swap_prev_hash;

	/* Hash table of agent numbers based on a coordinate and depth */
	std::unordered_map<int, int> occupied_coords;

	/* Path of an individual agent */
	std::stack<Coord> path;

	/* Iterate through the path of each agent */
	int num_agents = agents.size();
	for (int i = 0; i < num_agents; i++)
	{
		/* Get the path of the agent */
		path = agents[i]->get_solution();

		/* Initialize the depth */
		depth = 0;

		/* Get the coordinate and hash of the start coordinate */		
		prev_coord = path.top();
		prev_hash = CantorPair::get_int(&prev_coord, depth);
		path.pop();

		/* 
		* Iterate through each coordinate in the path except
		* the first coordinate because it is assumed the starting
		* coordinate of each agent is unique
		*/
		while (!path.empty())
		{
			/* Get the next coordinate in the path */
			curr_coord = path.top();
			path.pop();
			depth++;

			/* Get the hash of this coordinate and depth */
			hash = CantorPair::get_int(&curr_coord, depth);

			/* Check if that coordinate is already occupied at that depth */
			auto it = occupied_coords.find(hash);
			if (it != occupied_coords.end())
			{
				*agent_1 = i;
				*agent_2 = it->second;
				*conflict_1 = Position(curr_coord.get_xcoord(), curr_coord.get_ycoord(),depth);
				*conflict_2 = *conflict_1;
				return true;
			}

			/* Check if there is a 'swap' conflict */
			swap_curr_hash = CantorPair::get_int(&prev_coord, depth);
			swap_prev_hash = CantorPair::get_int(&curr_coord, depth - 1);
			auto swap_curr_it = occupied_coords.find(swap_curr_hash);
			auto swap_prev_it = occupied_coords.find(swap_prev_hash);
			if (
				swap_curr_it != occupied_coords.end() &&
				swap_prev_it != occupied_coords.end() &&
				swap_curr_it->second == swap_prev_it->second
				)
			{
				*agent_1 = i;
				*agent_2 = swap_curr_it->second;
				*conflict_1 = Position(curr_coord.get_xcoord(), curr_coord.get_ycoord(), depth);
				*conflict_2 = Position(prev_coord.get_xcoord(), prev_coord.get_ycoord(), depth);
				return true;
			}

			/* No conflict add this coordinate and depth to the hash table */
			occupied_coords.emplace(hash, i);

			/* Set the previous coord and hash to the current coord and hash */
			prev_coord = curr_coord;
			prev_hash = hash;
		}
	}
	
	/* No conflict was found */
	return false;
}

/* 
* Equals operator
* @param rhs: The CBSNode to copy
* @return the copied CBSNode
*/
CBSNode & CBSNode::operator=(CBSNode& rhs)
{
	/* Copy CBSNode data by value */
	agents = *rhs.get_agents();
	cost = rhs.get_cost();

	/* No new nodes generated in this node */
	new_agent_num = -1;

	return rhs;
}

/* 
* Destructor 
*/
CBSNode::~CBSNode()
{
	/* 
	* Delete the new agent generated in this node. The new_agent_num is -1
	* if no new agent was generated in this node 
	*/
	if (new_agent_num != -1)
		delete agents[new_agent_num];
}