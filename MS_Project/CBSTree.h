#ifndef CBSTREE_H
#define CBSTREE_H

#include <queue>
#include <string>
#include <functional>

class CBSNode;
class Agent;
class World;

class CBSTree
{
public:
	/* Constructor */
	CBSTree(std::string agent_file, std::string world_file);

	/* Get the solution of the MAPF problem */
	CBSNode* get_solution();
	
	/* Destructor */
	~CBSTree();
private:
	/* Minheap of CBSNodes, sorted by cost */
	std::priority_queue<CBSNode*, std::vector<CBSNode*>, std::greater<CBSNode>> tree;
	/* Vector of agents generated from a text file */
	std::vector<Agent*> agents;
	/* World the agents must explore */
	World* world;

	/* Generate an array of agents from a text file */
	void generate_agents(std::string txt_file);
	/* Convert a coordinate in the format ({int},{int}) to a Coord object */
	Coord str_to_coord(std::string coord_str);
};

#endif