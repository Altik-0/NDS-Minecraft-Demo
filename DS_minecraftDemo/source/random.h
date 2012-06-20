// random.h
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 18, 2012: File created
// ----------------------
// Defines the rand class, used to act as a rudimentary RNG.
//

#ifndef RANDOM_H
#define RANDOM_H

class random
{
public:
	int next();

	void setSeed(int _seed);
	void initSeed();

private:
	int seed;
};

#endif
