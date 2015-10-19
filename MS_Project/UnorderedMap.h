#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

#include <unordered_map>

/* Hash for unordered_map from the stl */
struct hash_struct {
	std::size_t operator()(const int key) const
	{
		return std::hash<int>()(key);
	}
};

#endif