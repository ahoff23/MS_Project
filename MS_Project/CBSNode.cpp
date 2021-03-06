#include "CBSNode.h"
#include "Agent.h"
#include "Coordinates.h"
#include "HashStruct.h"
#include "Exceptions.h"

/* 
* Constructor for the first node in the CBSTree
* (i.e. not based off of a parent CBSNode)
* @param p_agents: Pointer to a vector containing pointers to each agent
*/
#include <iostream>
CBSNode::CBSNode(std::vector<Agent*>* p_agents)
{
	/* Set the list of agents to point to the parameter list of agents */
	agents = *p_agents;

	/* No agents generated by this node (they are the responsibility of the calling CBSTree */
	new_agent_num = -1;
	
	/* Get solution and cost for all agents	*/
	cost = 0;
	int agent_cost;
	int num_agents = agents.size();
	for (int i = 0; i < num_agents; i++)
	{
		agent_cost = agents[i]->get_cost();
		if (agent_cost > cost)
			cost = agent_cost;
	}
}

/*
* Constructor for non-root nodes in the CBS tree
* @param parent_node: CBSNode to base this CBSNode on
* @param agent_num: The number of the agent who will have a conflict added
* to its list of conflicts
* @param conflict: The conflict to add to a single agent (agent_num)
*/
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
	cost = parent_node->get_cost();
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

	/* Depth of the search */
	int depth = 0;

	/* Hash table of agent numbers based on a coordinate and depth */
	std::unordered_multimap<unsigned int, AgentPos*> occupied_coords =
		std::unordered_multimap<unsigned int, AgentPos*>();

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

		/* Get the start coordinate */
		prev_coord = path.top();
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

			/* Create a position based on the coordinate and depth */
			Position curr_pos = Position(curr_coord, depth);

			/* Check if that coordinate is already occupied at that depth */
			auto it = find_it(&curr_pos, &occupied_coords);
			if (it != occupied_coords.end())
			{
				*agent_1 = i;
				*agent_2 = it->second->get_agent_num();

				/* Set agent 1's conflict */
				conflict_1->set_x(curr_coord.get_xcoord());
				conflict_1->set_y(curr_coord.get_ycoord());
				conflict_1->set_depth(depth);

				/* Agent 2 has the same conflict as agent 1*/
				*conflict_2 = *conflict_1;

				cleanup_occ_coords(&occupied_coords);
				return true;
			}

			/* Check if there is a 'swap' conflict */
			Position swap_curr_pos = Position(prev_coord, depth);
			Position swap_prev_pos = Position(curr_coord, depth - 1);
			
			auto swap_curr_it = find_it(&swap_curr_pos, &occupied_coords);
			auto swap_prev_it = find_it(&swap_prev_pos, &occupied_coords);
			if (
				swap_curr_it != occupied_coords.end() &&
				swap_prev_it != occupied_coords.end() &&
				swap_curr_it->second->get_agent_num() == swap_prev_it->second->get_agent_num()
				)
			{
				*agent_1 = i;
				*agent_2 = swap_curr_it->second->get_agent_num();

				/* Set agent 1's conflict */
				conflict_1->set_x(curr_coord.get_xcoord());
				conflict_1->set_y(curr_coord.get_ycoord());
				conflict_1->set_depth(depth);

				/* Set agent 2's conflict */
				conflict_2->set_x(prev_coord.get_xcoord());
				conflict_2->set_y(prev_coord.get_ycoord());
				conflict_2->set_depth(depth);
				cleanup_occ_coords(&occupied_coords);
				return true;
			}

			/* No conflict, add this coordinate and depth to the hash table */
			AgentPos* add_pos = new AgentPos(i,&curr_pos);
			occupied_coords.emplace(HashStruct::hash_pos(&curr_pos), add_pos);

			/* Set the previous coord and hash to the current coord and hash */
			prev_coord = curr_coord;
		}
	}
	/* No conflict was found */
	cleanup_occ_coords(&occupied_coords);
	return false;
}

/* 
* Cleanup occupied coords hash table 
* @param occupied_coords: Hash table to delete each AgentPos from
*/
void CBSNode::cleanup_occ_coords(
	std::unordered_multimap<unsigned int, AgentPos*>* occupied_coords
	)
{
	for (auto it = occupied_coords->begin(); it != occupied_coords->end(); it++)
		delete it->second;
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

/* Find a position in a hash table that allows some duplicates */
std::unordered_multimap<unsigned int, AgentPos*>::iterator CBSNode::find_it(
	Position* pos, std::unordered_multimap<unsigned int, AgentPos*>* map
	)
{
	/* Find the first iterator pointing to a key of pos */
	std::unordered_multimap<unsigned int, AgentPos*>::iterator it =
		map->find(HashStruct::hash_pos(pos));

	/* If no node contains this key, return a NULL pointer */
	if (it == map->end())
		return map->end();

	/* Store the key of the iterator */
	unsigned int hash_val = HashStruct::hash_pos(pos);

	/* Check all elements with this key */
	while (it->first == hash_val)
	{
		/* Compare coordinates and depth */
		if (*pos == *it->second->get_pos())
			return it;

		/* Move to the next element */
		it++;
	}

	/* The exact node could not be found */
	return map->end();
}

/*
* Print the solution to the console 
*/
void CBSNode::print_solution()
{
	for (int i = 0; i < agents.size(); i++)
		agents[i]->print_solution();
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