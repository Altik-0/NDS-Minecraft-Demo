// structures.h
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 18, 2012: File created
// ----------------------
// Contains simple data structures used throughout the program.
//

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <nds.h>

// Holds a 3 dimensional vector of integers
struct vec3i
{
	int x;
	int y;
	int z;
};

// Holds a 3 dimensional vector of v16s - effectively a single vertex in 3D space
struct vec3v16
{
	v16 x;
	v16 y;
	v16 z;
};

// Holds a 2 dimensional vector of integers
struct vec2i
{
	int x;
	int y;
};

// Enumerates all types of cubes. May change in the future, but there must be a power
// of two counts of cube types.
enum CUBETYPE
{
	air = 0, stone, dirt, grass,
};

// The number of types of cubes. This MUST be a power of two for &-mask mods to work.
#define NUM_CUBETYPES 4

// Represents all necessary data for a world cube
struct cube
{
	vec3i gridPosition;
	CUBETYPE type;
};

#endif
