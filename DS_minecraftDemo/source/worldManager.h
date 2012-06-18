#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#define MAX_CUBES 32768			// Maximum number of cubes allowable in the world

class worldManager
{
public:
	void intitialize();
	int size();

private:
	cube worldGrid[MAX_CUBES];
	int numCubes;
};

#endif
