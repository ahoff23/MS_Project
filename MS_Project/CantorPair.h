#ifndef CANTORPAIR_H
#define CANTORPAIR_H

class Position;
class AStarNode;
class Coord;

class CantorPair
{
public:
	static int get_int(Position* pos);
	static int get_int(AStarNode* node);
	static int get_int(Position* pos_1, Position* pos_2);
	static int get_int(Coord* coord, int depth);
	static int get_int(Coord* coord);
};

#endif