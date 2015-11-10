#ifndef HASHSTRUCT_H
#define HASHSTRUCT_H

class Position;
class Coord;

struct HashStruct
{
	/* Hashing functions for Positions and Coords */
	static unsigned int hash_pos(Position* key);
	static unsigned int hash_coord(Coord* key);

	/* Binary values where the first 11 and 10 digits, respectively, are all 1s */
	static const unsigned int ELEVEN;
	static const unsigned int TEN;

	/* Amount of digits to move for the coordinates and depth in a position hash */
	static const int POS_COORD_SHIFT;
	static const int POS_DEPTH_SHIFT;

	/* Amount of digits to move for the coordinates in a coordinate hash */
	static const int COORD_COORD_SHIFT;
};

#endif