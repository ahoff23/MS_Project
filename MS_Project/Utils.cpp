#include <sstream>

#include "Utils.h"

/* 
* Convert an int to a string 
* @param i: Int to convert to a string
* @return a string version of i
*/
std::string Utils::to_string(int i)
{
	/* Return string */
	std::ostringstream convert;
	convert << i;
	return convert.str();
}