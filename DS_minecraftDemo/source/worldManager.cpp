// worldManager.cpp
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 18, 2012: File created
// ----------------------
// Defines member functions of the worldManager class.
//

#include "worldManager.h"
#include "globals.h"
#include "random.h"

void worldManager::initialize()
{
	for(int z = 0; z < MAX_Z_CUBES; ++z)
	{
		for(int y = 0; y < MAX_Y_CUBES; ++y)
		{
			for(int x = 0; x < MAX_X_CUBES; ++x)
			{
				CUBETYPE _type = (CUBETYPE)(g_random.next() & (NUM_CUBETYPES-1));	// &-mask mod because NUM_CUBETYPES is guaranteed to be a power of two
				if(_type != air)
				{
					worldGrid[numCubes].gridPosition.x = x;
					worldGrid[numCubes].gridPosition.y = y;
					worldGrid[numCubes].gridPosition.z = z;
					worldGrid[numCubes].type = _type;

					++numCubes;
				}
			}
		}
	}
}

void worldManager::renderWorld()
{
	
	for(int i = 0; i < numCubes; ++i)
	{

	}
}

int worldManager::size()
{
	return numCubes;
}