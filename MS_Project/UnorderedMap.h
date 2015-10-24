#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

#include <unordered_map>

#include "CantorPair.h"
#include "Coordinates.h"

/* Hash for unordered_map from the stl */
struct hash_struct {
	std::size_t operator()(const int key) const
	{
		return std::hash<int>()(key);
	}
};

#endif