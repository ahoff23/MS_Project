#ifndef HASHSTRUCT_H
#define HASHSTRUCT_H

class Position;
class Coord;

struct HashStruct
{
	/* Hashing functions for Positions and Coords */
	static unsigned int hash_pos(Position* key);
	static unsigned int hash_coord(Coord* key);
	static unsigned short hash_coord_comp(Coord* main, Coord* comp);

	/* Convert a Coord comparison hash back to a Coord */
	static Coord hash_to_coord(unsigned short hash, Coord* comp_coord);

	/* Binary values where the first 11 and 10 digits, respectively, are all 1s */
	static const unsigned int ELEVEN;
	static const unsigned int TEN;

	/* Amount of digits to move for the coordinates and depth in a position hash */
	static const int POS_COORD_SHIFT;
	static const int POS_DEPTH_SHIFT;

	/* Amount of digits to move for the coordinates in a coordinate hash */
	static const int COORD_COORD_SHIFT;

	/* Bitmap values with a 1 bit shifted over */
	static const unsigned short BITMAP_VAL_0;
	static const unsigned short BITMAP_VAL_1;
	static const unsigned short BITMAP_VAL_2;
	static const unsigned short BITMAP_VAL_3;
	static const unsigned short BITMAP_VAL_4;
	static const unsigned short BITMAP_VAL_5;
	static const unsigned short BITMAP_VAL_6;
	static const unsigned short BITMAP_VAL_7;
	static const unsigned short BITMAP_VAL_8;
};

#endif