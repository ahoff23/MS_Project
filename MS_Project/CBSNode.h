#ifndef CBSNODE_H
#define CBSNODE_H

#include <vector>

class Agent;
class Position;

class CBSNode
{
public:
	/* Constructors */
	CBSNode(Agent** p_agents, int p_num_agents);
	CBSNode(CBSNode* parent_node, int agent_num, Position* conflict);

	/* Find one (or two) conflict positions between two agents */
	bool get_conflicts(int* agent_1, Position* conflict_1, int* agent_2, Position* conflict_2);

	/* Operator overloads */
	bool operator<(const CBSNode& comp) const;
	bool operator>(const CBSNode& comp) const;

	/* Accessors */
	int get_num_agents() { return num_agents; };
	Agent ** get_agents() { return agents; };

	/* Destructor */
	~CBSNode();
private:
	/* List of pointers to agents */
	Agent** agents;
	/* Number of agents in the node */
	int num_agents;
	/* Vector of agent numbers generated for this node */
	int new_agent_num;
	/* Cost of the node */
	int cost;
};

#endif