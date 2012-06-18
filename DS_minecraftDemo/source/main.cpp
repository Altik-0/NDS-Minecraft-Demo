// main.cpp
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 18, 2012: File created
// ----------------------
// Handles setting up the necessary internal memory registers and so forth to allow
// functioning on the NDS, as well as rendering all 3D objects.
//

// Used just to enable Visual Studio intellisense, because libnds is structured oddly
#ifndef ARM9
#define ARM9
#endif

#include <nds.h>
#include "globals.h"
#include "worldManager.h"

int main ()
{
	g_world.initialize();

	return 0;
}
