#ifndef Maze_h
#define Maze_h

/*Headers files*/
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

/* macros */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define SDL_WINDOW_HEIGHT 800
#define TEX_COUNT 6
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_CHAR_COUNT 1275

/* structure */
/**
 * struct point_s - data structure for XY point coordinates of type double
 * @x: X coordinate
 * @y: Y coordinate
 * Description: struct point
 */
typedef struct point_s
{
	double x;
	double y;
} point_t;

/* global variables */
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
extern uint32_t tiles[TEX_COUNT][TEX_HEIGHT][TEX_WIDTH];
extern point_t pos;
extern point_t dir;
extern point_t plane;
extern double time;

/* functions */
int *parseMap(char *filename, int *map);
void loadTextures(char *mapName);

bool initSDL(void);
void updateRenderer(bool textured);
void closeSDL(void);

void input(int *maze);
bool quit(void);

void raycaster(int *maze, bool textured);
void renderWalls(int *maze, SDL_Point map, point_t rayPos, point_t rayDir,
		double distToWall, int x, int side, bool textured);
void renderBGFlat(void);
void renderBGTex(SDL_Point map, point_t rayDir, double distToWall,
		double wallX, int drawEnd, int x, int side);

#endif
