#include "HashStruct.h"
#include "Coordinates.h"
#include "Exceptions.h"

/* Binary values where the first 11 and 10 digits, respectively, are all 1s */
const unsigned int HashStruct::ELEVEN = 2047;
const unsigned int HashStruct::TEN = 1023;

/* Amount of digits to move for the coordinates and depth in a position hash */
const int HashStruct::POS_COORD_SHIFT = 11;
const int HashStruct::POS_DEPTH_SHIFT = 10;

/* Amount of digits to move for the coordinates in a coordinate hash */
const int HashStruct::COORD_COORD_SHIFT = 16;

/* Bitmap values with a 1 bit shifted over */
const unsigned short HashStruct::BITMAP_VAL_0 = 1;
const unsigned short HashStruct::BITMAP_VAL_1 = 1 << 1;
const unsigned short HashStruct::BITMAP_VAL_2 = 1 << 2;
const unsigned short HashStruct::BITMAP_VAL_3 = 1 << 3;
const unsigned short HashStruct::BITMAP_VAL_4 = 1 << 4;
const unsigned short HashStruct::BITMAP_VAL_5 = 1 << 5;
const unsigned short HashStruct::BITMAP_VAL_6 = 1 << 6;
const unsigned short HashStruct::BITMAP_VAL_7 = 1 << 7;
const unsigned short HashStruct::BITMAP_VAL_8 = 1 << 8;

/* 
* Hash function to convert a Position to an unsigned int
* @param key: The position to get a hash for
* @return the hash value based on the key. Note that this is not guaranteed to be unique.
*/
unsigned int HashStruct::hash_pos(Position* key)
{
	/*
	* Set the hash value to the position's x coordinate
	* (later steps will leave only the lower 11 bits in hash_val)
	*/
	unsigned int hash_val = key->get_x_coord();

	/* Move the bits over to make room for 11 bits from the y coordinate */
	hash_val = hash_val << POS_COORD_SHIFT;

	/* Remove the extra digits (bits greater than 11) from the y coordinate */
	unsigned int y_coord = key->get_y_coord() & ELEVEN;

	/* Append the y values to hash_val */
	hash_val |= y_coord;

	/* Move the bits over to make room for 10 bits from the x coordinate */
	hash_val = hash_val << POS_DEPTH_SHIFT;

	/* Remove the extra digits (bits greater than 10) from the depth value */
	unsigned int depth = key->get_depth() & TEN;

	/* Append the depth to hash_val */
	hash_val |= depth;

	/* Get a hash value based on hash_val */
	return std::hash<unsigned int>()(hash_val);
}

/*
* Hash function to convert a Coordinate to an unsigned int
* @param key: The coordinate to get a hash for
* @return the hash value based on the key
*/
unsigned int HashStruct::hash_coord(Coord* key)
{
	/* Set the hash value to the x coordinate */
	unsigned int hash_val = key->get_xcoord();

	/* Move the x coordinate bits over to make room for the y coordinate */
	hash_val = hash_val << COORD_COORD_SHIFT;

	/* Append the y coordinate */
	hash_val |= key->get_ycoord();

	/* Get a hash value based on hash_val */
	return std::hash<unsigned int>()(hash_val);
}

/*
* Hash function based on a comparison of two coordinates
* @param main: The main coordinate to compare
* @param comp: The coordinate to compare to
* @return an unsigned short unique to the comparison of the two coordinates
*/
unsigned short HashStruct::hash_coord_comp(Coord* main, Coord* comp)
{
	/* Get the difference between the x and y coordinates */
	short x_diff = main->get_xcoord() - comp->get_xcoord();
	short y_diff = main->get_ycoord() - comp->get_ycoord();

	/* First compare by x coordinate */
	if (x_diff == -1)
	{
		if (y_diff == -1)
			return BITMAP_VAL_0;
		else if (y_diff == 0)
			return BITMAP_VAL_1;
		else if (y_diff == 1)
			return BITMAP_VAL_2;
		else
			throw TerminalException("Y coordinate difference greater than 1.");
	}
	else if (x_diff == 0)
	{
		if (y_diff == -1)
			return BITMAP_VAL_3;
		else if (y_diff == 0)
			return BITMAP_VAL_4;
		else if (y_diff == 1)
			return BITMAP_VAL_5;
		else
			throw TerminalException("Y coordinate difference greater than 1.");
	}
	else if (x_diff == 1)
	{
		if (y_diff == -1)
			return BITMAP_VAL_6;
		else if (y_diff == 0)
			return BITMAP_VAL_7;
		else if (y_diff == 1)
			return BITMAP_VAL_8;
		else
			throw TerminalException("Y coordinate difference greater than 1.");
	}
	else
		throw TerminalException("X coordinate difference greater than 1.");
}


/* 
* Convert a Coord comparison hash back to a Coord 
* @param hash: The hash to convert to a coordinate
* @param comp_coord: The Coord to use as a comparison
* @return a Coord based on hash
*/
Coord HashStruct::hash_to_coord(unsigned short hash, Coord* comp_coord)
{
	/* Get the x and y values of comp_coord */
	unsigned short x_coord = comp_coord->get_xcoord();
	unsigned short y_coord = comp_coord->get_ycoord();

	if (BITMAP_VAL_0 == hash)
		return Coord(x_coord - 1, y_coord - 1);
	else if (BITMAP_VAL_1 == hash)
		return Coord(x_coord - 1, y_coord);
	else if (BITMAP_VAL_2 == hash)
		return Coord(x_coord - 1, y_coord + 1);
	else if (BITMAP_VAL_3 == hash)
		return Coord(x_coord, y_coord - 1);
	else if (BITMAP_VAL_4 == hash)
		return Coord(x_coord, y_coord);
	else if (BITMAP_VAL_5 == hash)
		return Coord(x_coord, y_coord + 1);
	else if (BITMAP_VAL_6 == hash)
		return Coord(x_coord + 1, y_coord - 1);
	else if (BITMAP_VAL_7 == hash)
		return Coord(x_coord + 1, y_coord);
	else if (BITMAP_VAL_8 == hash)
		return Coord(x_coord + 1, y_coord + 1);
	else
		throw TerminalException("Bad hash value when converting back to Coord.");
}