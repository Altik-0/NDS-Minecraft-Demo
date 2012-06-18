#ifndef STRUCTURES_H
#define STRUCTURES_H

// Holds a 3 dimensional vector of integers
struct vec3i
{
	int x;
	int y;
	int z;
};

// Holds a 2 dimensional vector of integers
struct vec2i
{
	int x;
	int y;
};

// Enumerates all types of cubes
enum CUBETYPE
{
	air = -1, stone, dirt, grass,
};

// Represents all necessary data for a world cube
struct cube
{
	vec3i gridPosition;
	CUBETYPE type;
};

#endif
