#ifndef GLOBALS_H
#define GLOBALS_H

struct vector;

union SpriteEntry;

extern SpriteEntry* g_highlighter;
extern u16* highlighterTilesLoc;

extern u16** widthDotsLoc;
extern int widthSelection;
extern int width;

extern u16* colorGrid;
extern vector colorGridSelection;

#define color_buttonTilesLen 1024
extern const unsigned short color_buttonTiles[512];

#define highlighterTilesLen 1024
extern const unsigned short highlighterTiles[512];

#define instructionsTilesLen 1024
extern const unsigned short instructionsTiles[512];

#define width_4TilesLen 32
extern const unsigned short width_4Tiles[16];
#define width_3TilesLen 32
extern const unsigned short width_3Tiles[16];
#define width_2TilesLen 32
extern const unsigned short width_2Tiles[16];
#define width_1TilesLen 32
extern const unsigned short width_1Tiles[16];

#define sprite_PalLen 512
extern const unsigned short sprite_Pal[256];

#endif