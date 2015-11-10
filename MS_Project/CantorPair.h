#ifndef CANTORPAIR_H
#define CANTORPAIR_H

class Position;
class AStarNode;
class Coord;

class CantorPair
{
public:
	static unsigned int get_int(Position* pos);
	static unsigned int get_int(AStarNode* node);
	static unsigned int get_int(Position* pos_1, Position* pos_2);
	static unsigned int get_int(Coord* coord, unsigned short depth);
	static unsigned int get_int(Coord* coord);
};

#endif