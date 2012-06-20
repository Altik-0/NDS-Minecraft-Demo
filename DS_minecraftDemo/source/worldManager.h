// worldManager.cpp
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 18, 2012: File created
// ----------------------
// Defines the worldManager class.
//

#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include "structures.h"

#define MAX_X_CUBES 32

#define MAX_Y_CUBES 32
#define MAX_Z_CUBES 32
#define MAX_CUBES MAX_X_CUBES*MAX_Y_CUBES*MAX_Z_CUBES			// Maximum number of cubes allowable in the world

class worldManager
{
public:
	void initialize();
	void renderWorld();

	inline int size();

private:
	cube worldGrid[MAX_CUBES];
	int numCubes;
};

#endif
