#ifndef CANTORPAIR_H
#define CANTORPAIR_H

class Position;
class AStarNode;

class CantorPair
{
public:
	static int get_int(Position* pos);
	static int get_int(AStarNode* node);
};

#endif