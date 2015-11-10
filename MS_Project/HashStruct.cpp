#include "HashStruct.h"
#include "Coordinates.h"

/* Binary values where the first 11 and 10 digits, respectively, are all 1s */
const unsigned int HashStruct::ELEVEN = 2047;
const unsigned int HashStruct::TEN = 1023;

/* Amount of digits to move for the coordinates and depth in a position hash */
const int HashStruct::POS_COORD_SHIFT = 11;
const int HashStruct::POS_DEPTH_SHIFT = 10;

/* Amount of digits to move for the coordinates in a coordinate hash */
const int HashStruct::COORD_COORD_SHIFT = 16;

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

#include <iostream>
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