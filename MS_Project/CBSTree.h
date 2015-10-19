#ifndef CBSTREE_H
#define CBSTREE_H

#include <queue>
#include <string>
#include <functional>

class CBSNode;
class Agent;
class World;
class Coord;

/* Struct for comparing two CBSNodes by cost */
struct Compare : public std::binary_function<CBSNode*, CBSNode*, bool>
{
	bool operator()(const CBSNode* lhs, const CBSNode* rhs) const;
};

class CBSTree
{
public:
	/* Constructor */
	CBSTree(std::string agent_file, std::string world_file);
	/* Get the solution of the MAPF problem */
	CBSNode* get_solution();
	/* Print the solution of the tree to an output file */
	void file_print_solution();
	
	/* Destructor */
	~CBSTree();
private:
	/* Minheap of CBSNodes, sorted by cost */
	std::priority_queue<CBSNode*, std::vector<CBSNode*>, Compare> tree;
	/* Vector of agents generated from a text file */
	std::vector<Agent*> agents;
	/* World the agents must explore */
	World* world;
	/* CBSNodes that were previously explored */
	std::vector<CBSNode*> closed_nodes;

	/* Generate an array of agents from a text file */
	void generate_agents(std::string txt_file);
	/* Convert a coordinate in the format ({int},{int}) to a Coord object */
	Coord* str_to_coord(std::string coord_str);
};

#endif