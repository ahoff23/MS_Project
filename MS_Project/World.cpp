#include <fstream>
#include <iostream>

#include "World.h"
#include "Coordinates.h"
#include "Exceptions.h"

World::World()
{
	/* Default values not possible values for error checking */
	coords = std::vector<bool>();
	max_x = 0;
	max_y = 0;
}

/*
* Constructor for a world. Creates the world by reading from a text file.
* The text file must be formatted such that obstacles are represented by 0s and
* open spaces are reperesented by 1s. No other characters are accepted, but
* the lines may be variable length so long they begin at the beginning of a line.
* @param txt_file: The name of the text file to read
* @exceptions: Throws TerminalExceptions if the file cannot be read or does not meet
* the appropriate format standards
*/
World::World(std::string txt_file)
{
	/* Open the text file */
	std::ifstream world_file(txt_file.c_str());
	if (!world_file.is_open())
		throw TerminalException("World file could not be opened.");

	/* ASCII value for carriage return */
	const int CARRIAGE_RETURN = 13;

	/* Default values for max_x and max_y */
	max_x = 0;
	max_y = 0;

	/* Read the file once to get max_x and max_y */
	std::string line;
	while (getline(world_file, line))
	{
		max_y++;

		/* Line length if the first character is at index 0 */
		unsigned short line_length = static_cast<unsigned short>(line.length());

		/* If the final character is a newline, '\n', decrement the line length to ignore it */
		if (line[line_length - 1] == CARRIAGE_RETURN)
			line_length--;

		if (max_x < line_length)
			max_x = line_length;
	}

	/* Make sure the world has at least one coordinate */
	if (max_x == 0 || max_y == 0)
		throw TerminalException("World text file must have at least one coordinate.");

	/* Initialize the vector of coordinates */
	coords = std::vector<bool>();

	/* max_x and max_y should be decremented to signify the largest index for either */
	max_x--;
	max_y--;

	/* Integer tracking which element of coords should be set next */
	int coords_index = 0;

	/* Return to the beginning of the file */
	world_file.clear();
	world_file.seekg(0, std::ios::beg);
	std::cout << "";

	/* Check each row of the world in the file */
	while (getline(world_file, line))
	{
		/* Length of the line */
		unsigned short line_length = static_cast<unsigned short>(line.length());

		/* Read each entry in the line (i is the column number) */
		for (int i = 0; i < max_x + 1; i++)
		{
			/* Stop reading the line if at the end of the line */
			if (i >= line_length || line.at(i) == CARRIAGE_RETURN)
				break;

			if (line.at(i) == '0')
				coords.push_back(false);
			else if (line.at(i) == '1')
				coords.push_back(true);
			else
				throw TerminalException("Invalid file format.");
		}

		/* 
		* If the number of entries in this row is less than the max number of entries
		* in any row, then append obstacles until the length of this row equals the maximum
		* row length. This makes the matrix a square matrix, but allows the user to make
		* worlds with variable sized rows.
		*/
		for (int i = line_length; i <= max_x; i++)
			coords.push_back(false);
	}
	world_file.close();
}

/* 
* Check if a coordinate is open and that it exists 
* @param coord: The coordinate to check for in the function.
* @return true if the coordinate exists and is not blocked by an obstacle,
* otherwise return false.
*/
bool World::check_coord(Coord* coord)
{
	/* Get the X and Y coordinates of the coord */
	unsigned short x_coord = coord->get_xcoord();
	unsigned short y_coord = coord->get_ycoord();

	/* Check if the coordinate exists */
	if (x_coord > max_x || y_coord > max_y || x_coord < 0 || y_coord < 0)
		return false;
	
	/* Find the index of the coordinate in the coords matrix */
	int index = y_coord * (max_x + 1) + x_coord;
	
	/* Check if the coordinate is free */
	return coords[index];
}

/* 
* Print the World to the console 
*/
void World::print_world()
{
	/* Initialize the x coordinate tracker */
	unsigned short x_coord = 0;

	/* Iterate through each element in coords */
	for (int i = 0; i < (max_x + 1) * (max_y + 1); i++)
	{
		/* Check for a newline */
		if (x_coord > max_x)
		{
			std::cout << std::endl;
			x_coord = 0;
		}

		/* Print 1 for an open space, 0 for an obstacle */
		coords[i] == true ? std::cout << 1 : std::cout << 0;

		x_coord++;
	}
	std::cout << std::endl;
}