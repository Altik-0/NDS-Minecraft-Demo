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
#include "random.h"

void video_init();
void drawScene();

int main ()
{
	video_init();
	g_world.initialize();

	while(1)
	{
		// Key detection - TODO

		// Render 3D components
		drawScene();

		// Draw to screens
		swiWaitForVBlank();
		glFlush(0);
	}

	return 0;
}

void video_init()
{
	// Set video registers
	videoSetMode(MODE_0_3D);
	//videoSetModeSub(MODE_0_2D);

	//---------------------
	// Setup OpenGL stuff
	//---------------------
	glInit();
	glEnable(GL_ANTIALIAS);

	// Screen must be opaque; AA requires unique poly ID; setup Z-Buffer
	glClearColor(0, 0, 0, 31);
	glClearPolyID(63);
	glClearDepth(0x7FFF);

	// Set viewport to same size as screen
	glViewport(0, 0, 255, 191);

	// Setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		/* Parameters are in fixed point. Here is a translation:
			Field of View: 70 degrees
			Aspect Ratio: 256/192 (screen dimensions)
			nearZ: 0.1
			farZ: 100.0		*/
	gluPerspectivef32(286720,5461,409,409600);

	//------------------------------
	// Setup bottom-screen sprites
	//------------------------------

	// TODO
}

void drawScene()
{
	// DS-specific formatting
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);

	// Setup MV matrix
	glMatrixMode(GL_MODELVIEW);

	// Save the MV matrix
	glPushMatrix();

	// Draw Polys
		//TODO

	// Reload the MV matrix
	glPopMatrix(1);
}
