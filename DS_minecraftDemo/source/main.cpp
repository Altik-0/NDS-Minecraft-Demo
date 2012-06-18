// main.cpp
//
// ----------------------
// Author:
//		Aric Parkinson
//
// Modification notes:
//	  - June 4, 2012:  File created
//	  - Jume 5, 2012:  Implemented simple doodle application using just the bottom screen display.
//					   not especially impressive yet, and looks crappy with high-speed drawing, but
//					   it does work decently well.
//    - June 6, 2012:  Bresenham lines implemented: draws between two selected points a one pixel
//					   width line. Doesn't work for lines that are steeper than +/- 0.5 in slope,
//					   however.
//	  - June 7, 2012:  Updated the Bresenham Lines algorithm to function using fixed point arithmetic
//					   rather than hardware unsupported floats.
//					   Fixed the program back to doodling - now does interpolated lines, so drawing
//					   quickly doesn't make the line look really bad anymore. :D
//	  - June 10, 2012: Discovered that libnds doesn't like you building your own OAM Manager, so I
//					   caved and used theirs. Unfortuantely this leads to a lot of messiness too. If
//					   I ever decide to go for a large-scale project for the NDS, I'll probably need
//					   to write my own library to handle it... ugh...
//					   That said, I got the sprites displaying and the highlighter works. Just need
//					   to work out changing the color for drawing, and then maybe width and I'll be
//					   finished!
// ----------------------
// Initializes all necessary components for the application to function properly on the NDS.
//

// TODO: Remove this line at the end of production: used just to enable intellisense.
#ifndef ARM9
#define ARM9
#endif

#include <nds.h>
#include "globals.h"
#include "vector.h"

// Used to keep track of current touchscreen I/O information
touchPosition TOUCH_DATA;
vector TOUCH_PREV;
vector TOUCH_CURR;

#define REG_DISPCNT_B ((u16*) (0x04001000))

// The NDS doesn't have floating point hardware, but libnds provides a fixed point math library
// so I'm just typedefing a 32 bit integer as "fixed" to keep things clear later in code.
// Expected use is 20.12 format: ie, 20 integer bits, 12 fractional bits
typedef int fixed;

// Used by nextRandNum() to determine the next value in the sequence. Changed with each call
// to nextRandNum(), as well as slightly modified by each button press or touchscreen press.
int SEED = 0;

// Returns the next random number in a sequence based on the current global random seed value.
int nextRandNum();

// Checks if a button was pressed or if the touchscreen was touched, and returns true if this
// is the case. Also slightly modifies the random SEED value based on the user input to provide
// some extra spontaneity.
bool checkButtonPress();

// Draws a line between TOUCH_CURR and TOUCH_PREV of the specified width. Width must be greater
// than 0, and odd
void drawBresenhamLine(u16 color, int width = 1);

// Draws circular caps for the lines going from TOUCH_CURR to TOUCH_PREV
void drawCircleCaps(u16 color, int radius = 0);

// Draws the color at the specified pixel position into VRAM_A
void draw(int x, int y, u16 color);

// Initialzes all video components
void videoInit();

// Initialize color grid
void colorGridInit();

int main ()
{
	// Turn power on to necessary hardware components
	//powerOn(POWER_ALL_2D);
	
	// Initialize video
	videoInit();
	colorGridInit();

	// Begin main loop - DOODLING
	while(1)
	{
		// Check key press
		scanKeys();

		// Determine drawing width
		switch(widthSelection)
		{
		case 8:
			width = 1;
			break;
		case 4:
			width = 3;
			break;
		case 2:
			width = 5;
			break;
		case 1:
			width = 7;
			break;
		}

		// If the touchpad was just barely touched, we don't want to draw between the points
		if(keysDown() & KEY_TOUCH)
		{
			touchRead(&TOUCH_DATA);
			TOUCH_CURR.x = TOUCH_DATA.px;
			TOUCH_CURR.y = TOUCH_DATA.py;
		}
		// As we draw, we want to interpolate the line between the two draw points - ie, draw
		// a bresenham line with circular caps
		else if(keysHeld() & KEY_TOUCH)
		{
			touchRead(&TOUCH_DATA);
			TOUCH_PREV.x = TOUCH_CURR.x;
			TOUCH_PREV.y = TOUCH_CURR.y;
			TOUCH_CURR.x = TOUCH_DATA.px;
			TOUCH_CURR.y = TOUCH_DATA.py;

			drawBresenhamLine(colorGrid[(colorGridSelection.y*2)+colorGridSelection.x], width);
			drawCircleCaps(colorGrid[(colorGridSelection.y*2)+colorGridSelection.x], (width/2));
		}

		// Press select to clear the screen
		if(keysDown() & KEY_SELECT)
		{
			for(int y = 0; y < SCREEN_HEIGHT; ++y)
			{
				for(int x = 0; x < SCREEN_WIDTH; ++x)
				{
					draw(x, y, RGB15(0,0,0));
				}
			}
		}

		// Press L/R to adjust width
		if(keysDown() & KEY_L)
		{
			if(widthSelection == 8)
			{
				widthSelection = 1;
			}
			else
			{
				widthSelection <<= 1;
			}
		}
		if(keysDown() & KEY_R)
		{
			if(widthSelection == 1)
				widthSelection = 8;
			else
				widthSelection >>= 1;
		}

		// Press D-Pad to move the highlighter and change colors
		if(keysDown() & KEY_DOWN)
		{
			if(g_highlighter->y == 154)
			{
				g_highlighter->y = 6;
				colorGridSelection.y = 0;
			}
			else
			{
				g_highlighter->y += 37;
				colorGridSelection.y += 1;
			}
		}
		if(keysDown() & KEY_UP)
		{
			if(g_highlighter->y == 6)
			{
				g_highlighter->y = 154;
				colorGridSelection.y = 4;
			}
			else
			{
				g_highlighter->y -= 37;
				colorGridSelection.y -= 1;
			}
		}
		if(keysDown() & KEY_RIGHT)
		{
			g_highlighter->x = (g_highlighter->x == 6) ? 80 : 6;
			colorGridSelection.x = (colorGridSelection.x == 0) ? 1 : 0;
		}
		if(keysDown() & KEY_LEFT)
		{
			g_highlighter->x = (g_highlighter->x == 6) ? 80 : 6;
			colorGridSelection.x = (colorGridSelection.x == 0) ? 1 : 0;
		}

		// Update the position of the highlighter
		oamSet(&oamSub, 10,
			   g_highlighter->x, g_highlighter->y,
			   0, 0,
			   SpriteSize_64x32,
			   SpriteColorFormat_16Color,
			   highlighterTilesLoc,
			   -1,
			   false,
			   false,
			   false, false,
			   false);
		// Update the highlight for the width dots
		oamSet(&oamSub, 12,
		   156, 25,
		   0, ((~widthSelection) & 8) >> 3,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[0],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
		oamSet(&oamSub, 13,
		   161, 25,
		   0, ((~widthSelection) & 4) >> 2,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[1],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
		oamSet(&oamSub, 14,
		   169, 25,
		   0, ((~widthSelection) & 2) >> 1,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[2],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
		oamSet(&oamSub, 15,
		   178, 25,
		   0, ((~widthSelection) & 1),
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[3],
		   -1,
		   false,
		   false,
		   false, false,
		   false);

		swiWaitForVBlank();

		// Update OAM
		oamUpdate(&oamSub);
	}

	return 0;
}

void colorGridInit()
{
	colorGrid = new u16[10];
	colorGrid[(0*2)+0] = 0x001F;
	colorGrid[(1*2)+0] = 0x01FF;
	colorGrid[(2*2)+0] = 0x03FF;
	colorGrid[(3*2)+0] = 0x03E0;
	colorGrid[(4*2)+0] = 0x7C00;

	colorGrid[(0*2)+1] = 0x7C0F;
	colorGrid[(1*2)+1] = 0x0000;
	colorGrid[(2*2)+1] = 0x318C;
	colorGrid[(3*2)+1] = 0x7FFF;
	colorGrid[(4*2)+1] = 0x00CC;

	colorGridSelection.x = 0;
	colorGridSelection.y = 0;
}

void videoInit()
{
	// Set up video mode
	videoSetMode(MODE_FB0);		// Using bitmap-mode in VRAM_A for main display (bottom screen)
	videoSetModeSub(MODE_0_2D);	// Using standard sprites for sub display (top screen)

	// Set up VRAM banks
	vramSetBankA(VRAM_A_LCD);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	// Initialize OAM
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	// Setup sprites
		// --- Buttons ---
	u16* gfxLoc = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, color_buttonTiles, gfxLoc, color_buttonTilesLen);

	for(int i = 0; i < 5; ++i)
	{
		// Right sprite
		oamSet(&oamSub, (i << 1),
			   6, 6 + (32 * i) + (5 * i),
			   1, i,
			   SpriteSize_64x32,
			   SpriteColorFormat_16Color,
			   gfxLoc,
			   -1,
			   false,
			   false,
			   false, false,
			   false);

		// Left sprite
		oamSet(&oamSub, (i << 1) + 1,
			   80, 6 + (32 * i) + (5 * i),
			   1, i + 5,
			   SpriteSize_64x32,
			   SpriteColorFormat_16Color,
			   gfxLoc,
			   -1,
			   false,
			   false,
			   false, false,
			   false);
	}

		// --- Instructions ---
	gfxLoc = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, instructionsTiles, gfxLoc, instructionsTilesLen);
	oamSet(&oamSub, 11,
		   174, 154,
		   0, 0,
		   SpriteSize_64x32, SpriteColorFormat_16Color,
		   gfxLoc,
		   -1,
		   false,
		   false,
		   false, false,
		   false);

		// --- Width indicators ---
	widthDotsLoc = new u16*[4];
	widthDotsLoc[0] = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, width_1Tiles, widthDotsLoc[0], width_1TilesLen);
	oamSet(&oamSub, 12,
		   156, 25,
		   0, 1,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[0],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
	widthDotsLoc[1] = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, width_2Tiles, widthDotsLoc[1], width_2TilesLen);
	oamSet(&oamSub, 13,
		   161, 25,
		   0, 0,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[1],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
	widthDotsLoc[2] = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, width_3Tiles, widthDotsLoc[2], width_3TilesLen);
	oamSet(&oamSub, 14,
		   169, 25,
		   0, 1,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[2],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
	widthDotsLoc[3] = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, width_4Tiles, widthDotsLoc[3], width_4TilesLen);
	oamSet(&oamSub, 15,
		   178, 25,
		   0, 1,
		   SpriteSize_8x8, SpriteColorFormat_16Color,
		   widthDotsLoc[3],
		   -1,
		   false,
		   false,
		   false, false,
		   false);
	
	widthSelection = 4;
	width = 2;

		// --- Highlighter ---
	highlighterTilesLoc = oamAllocateGfx(&oamSub, SpriteSize_64x32, SpriteColorFormat_16Color);
	dmaCopyHalfWords(3, highlighterTiles, highlighterTilesLoc, highlighterTilesLen);
	oamSet(&oamSub, 10,
		   6, 6,
		   0, 0,
		   SpriteSize_64x32,
		   SpriteColorFormat_16Color,
		   highlighterTilesLoc,
		   -1,
		   false,
		   false,
		   false, false,
		   false);
	g_highlighter = new SpriteEntry;
	g_highlighter->x = 6;
	g_highlighter->y = 6;


	// Setup palettes
	dmaCopyHalfWords(3, sprite_Pal, SPRITE_PALETTE_SUB, sprite_PalLen);

	// Put the main display on the bottom screen for more convenient drawing
	lcdSwap();

	// initial screen clear for bottom screen
	u16* buffer = VRAM_A;
	for(int y = 0; y < 192; ++y)
	{
		for(int x = 0; x < 256; ++x)
		{
			*buffer++ = RGB15(0, 0, 0);
		}
	}

	// initial screen clear for top screen
	buffer = VRAM_C;
	for(; (int)buffer != (int)VRAM_D; buffer++)
	{
		*buffer = 0;
	}
}

bool checkButtonPress()
{
	// Get current status of the input
	scanKeys();

	bool isPressed = false;
	u32 KEY_PRESS = keysDown();
	
	if(KEY_PRESS & ~KEY_TOUCH)		// If any NON-TOUCH keys are pressed
	{
		isPressed = true;
		SEED ^= KEY_PRESS;
	}
	if(KEY_PRESS & KEY_TOUCH)		// If the touchpad is pressed
	{
		isPressed = true;

		TOUCH_PREV.x = TOUCH_CURR.x;
		TOUCH_PREV.y = TOUCH_CURR.y;
		touchRead(&TOUCH_DATA);
		TOUCH_CURR.x = TOUCH_DATA.px;
		TOUCH_CURR.y = TOUCH_DATA.py;

		SEED += TOUCH_CURR.x;
		SEED -= TOUCH_CURR.y;
	}

	return isPressed;
}

void drawBresenhamLine(u16 color, int width)
{
	int x0 = TOUCH_PREV.x;
	int x1 = TOUCH_CURR.x;
	int y0 = TOUCH_PREV.y;
	int y1 = TOUCH_CURR.y;

	fixed dx = (x1 - x0) << 12;
	if(dx < 0.0f)
		dx = -dx;
	fixed dy = (y1 - y0) << 12;
	if(dy < 0.0f)
		dy = -dy;

	bool isSteep = dy > dx;

	// We need to be able to handle lines that are going steeply up, so if that is the
	// case, we will swap the x and y values to pretend we've reflected it to have a 
	// non-steep slope
	if(isSteep)
	{
		int tmp = x0;
		x0 = y0;
		y0 = tmp;
		tmp = x1;
		x1 = y1;
		y1 = tmp;
	}

	// We want to step going left to right, so if x0 is farther to the right, we will
	// swap the points
	if(x0 > x1)
	{
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	dx = x1 - x0;
	dy = y1 - y0;
	fixed m = divf32(dy, dx);

	// Absolute value
	if(m < 0)
		m = -m;

	fixed error = 0;
	int y = y0;
	int direction = (y1 > y0) ? 1 : -1;

	int halfWidth = width >> 1;

	// Base line draw
	// Draw each pixel
	for (int x = x0; x <= x1; ++x)
	{
		if(!isSteep)
		{
			for(int i = 0; i < width; ++ i)
			{
				draw(x, (y + halfWidth - i), color);
			}
		}
		else
		{
			for(int i = 0; i < width; ++ i)
			{
				draw((y + halfWidth - i), x, color);
			}
		}
		
		error = error + m;
		if(error >= 0x0800)				// 0x0800 is 1/2 in 20.12 fixed point
		{
			y += direction;
			error = error - (1 << 12);	// (1 << 12) is 1.0 in 20.12 fixed point
		}
	}
}

void drawCircleCaps(u16 color, int radius)
{
	// --- Draw the TOUCH_CURR cap ---
	vector center;

	center.x = TOUCH_CURR.x;
	center.y = TOUCH_CURR.y;
	// Define bounding box, relying on 4-way symmetry
	// Make checks
	for(int y = 0; y <= radius; ++y)
	{
		for(int x = 0; x <= radius; ++x)
		{
			fixed norm = sqrtf32(((x * x) << 12) + ((y * y) << 12));
			fixed difference = norm - (radius << 12);
			if(difference <= 0x0800)		// 0x0800 is 1/2 in 20.12 fixed point
			{
				draw(x + center.x, y + center.y, color);
				draw(center.x - x, y + center.y, color);
				draw(x + center.x, center.y - y, color);
				draw(center.x - x, center.y - y, color);
			}
		}
	}

	// --- Draw the TOUCH_PREV cap ---
	center.x = TOUCH_PREV.x;
	center.y = TOUCH_PREV.y;

	// Define bounding box, relying on 4-way symmetry
	// Make checks
	for(int y = 0; y <= radius; ++y)
	{
		for(int x = 0; x <= radius; ++x)
		{
			fixed norm = sqrtf32(((x * x) << 12) + ((y * y) << 12));
			fixed difference = norm - (radius << 12);
			if(difference <= 0x0800)		// 0x0800 is 1/2 in 20.12 fixed point
			{
				draw(x + center.x, y + center.y, color);
				draw(center.x - x, y + center.y, color);
				draw(x + center.x, center.y - y, color);
				draw(center.x - x, center.y - y, color);
			}
		}
	}
}

void draw(int x, int y, u16 color)
{
	if(x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0)
		return;
	u16* buffer = VRAM_A;
	buffer[(y * 256) + x] = color;
}