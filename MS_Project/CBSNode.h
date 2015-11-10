#ifndef CBSNODE_H
#define CBSNODE_H

#include <vector>
#include <unordered_map>

class Agent;
class Position;
class AgentPos;

class CBSNode
{
public:
	/* Constructors */
	CBSNode(std::vector<Agent*>* p_agents);
	CBSNode(CBSNode* parent_node, int agent_num, Position* conflict);

	/* Find one (or two) conflict positions between two agents */
	bool get_conflicts(int* agent_1, Position* conflict_1, int* agent_2, Position* conflict_2);
	/* Find a position in a hash table that allows some duplicates */
	std::unordered_multimap<unsigned int, AgentPos*>::iterator
		find_it(Position* pos, std::unordered_multimap<unsigned int, AgentPos*>* map);

	/* Operators */
	CBSNode & operator=(CBSNode& rhs);

	/* Accessors */
	int get_num_agents() const { return agents.size(); };
	int get_cost() const { return cost; };
	std::vector<Agent*>* get_agents() { return &agents; };

	/* Destructor */
	~CBSNode();
private:
	/* Vector of pointers to agents */
	std::vector<Agent*> agents;
	/* Vector of agent numbers generated for this node */
	int new_agent_num;
	/* Cost of the node */
	int cost;
	/* Cleanup occupied coords hash table */
	void cleanup_occ_coords(
		std::unordered_multimap<unsigned int, AgentPos*>* occupied_coords
		);
};

#endif