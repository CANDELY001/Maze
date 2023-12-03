#include "Maze.h"
/**
 * renderWalls - draws slice of wall to buffer
 * @maze: 2D array defining maze map
 * @map: X/Y coordinates of box of maze currently in
 * @rayPos: X/Y coordinates of ray position
 * @rayDir: direction of X/Y coordinates of ray position
 * @distToWall: distance to wall from camera
 * @x: number of ray casted
 * @side: determines whether wall is N/S or E/W
 * @textured: True if user enabled textures, otherwise False
 * Return: oid
 */
void renderWalls(int *maze, SDL_Point map, point_t rayPos, point_t rayDir,
		double distToWall, int x, int side, bool textured)
{
	int sliceH, drawStart, drawEnd, tileIndex, y;
	double wallX;
	SDL_Point tex;
	uint32_t color;

	sliceH = (int)(textured ? SCREEN_HEIGHT : SDL_WINDOW_HEIGHT) / distToWall;
	drawStart = -sliceH / 2 + (textured ? SCREEN_HEIGHT : SDL_WINDOW_HEIGHT) / 2;
	drawStart = (drawStart < 0) ? 0 : drawStart;
	drawEnd = sliceH / 2 + (textured ? SCREEN_HEIGHT : SDL_WINDOW_HEIGHT) / 2;
	drawEnd = (drawEnd >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - 1 : drawEnd;

	wallX = (side == 0) ? rayPos.y + distToWall * rayDir.y
		: rayPos.x + distToWall * rayDir.x;
	wallX -= floor(wallX);

	tileIndex = *((int *)maze + map.x * MAP_WIDTH + map.y) - 1;
	tex.x = (int)(wallX * (double)TEX_WIDTH);
	tex.x = (side == 0 && rayDir.x > 0) ? TEX_WIDTH - tex.x - 1 : tex.x;
	tex.x = (side == 1 && rayDir.y < 0) ? TEX_WIDTH - tex.x - 1 : tex.x;

	for (y = drawStart; y < drawEnd; y++)
	{
		tex.y = ((((y << 1) - SCREEN_HEIGHT + sliceH)
					<< (int)log2(TEX_HEIGHT)) / sliceH) >> 1;
		color = tiles[tileIndex][tex.x][tex.y];
		color = (side == 1) ? ((color >> 1) & 8355711) : color;
		buffer[y][x] = color;
	}

	if (textured)
		renderBGTex(map, rayDir, distToWall, wallX, drawEnd, x, side);
	else
		SDL_SetRenderDrawColor(renderer, (side == 0) ? 0xF7
				: 0xDE, (side == 0) ? 0xF7 : 0xDE, (side == 0) ? 0xF7 : 0xDE, 0xFF);

	SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
}
/**
 * renderBGTex - draws floor and ceiling with textures
 * @map: X/Y coordinates of box of maze currently in
 * @rayDir: direction of X/Y coordinates of ray position
 * @distToWall: distance to wall from camera
 * @wallX: X position of where wall was hit by raycaster
 * @drawEnd: end position of wall slice that was drawn
 * @x: number of ray casted
 * @side: determines whether wall is N/S or E/W
 * Return: void
 */
void renderBGTex(SDL_Point map, point_t rayDir,
		double distToWall, double wallX, int drawEnd, int x, int side)
{
	point_t floorWall;
	point_t currentFloor;
	SDL_Point floorTex;
	double weight;
	double currentDist;
	int y;

	if (side == 0 && rayDir.x > 0)
		floorWall.x = map.x, floorWall.y = map.y + wallX;
	else if (side == 0 && rayDir.x < 0)
		floorWall.x = map.x + 1.0, floorWall.y = map.y + wallX;
	else if (side == 1 && rayDir.y > 0)
		floorWall.x = map.x + wallX, floorWall.y = map.y;
	else
		floorWall.x = map.x + wallX, floorWall.y = map.y + 1.0;
	if (drawEnd < 0)
		drawEnd = SCREEN_HEIGHT;
	for (y = drawEnd + 1; y < SCREEN_HEIGHT; y++)
	{
		currentDist = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
		weight = currentDist / distToWall;
		currentFloor.x = weight * floorWall.x + (1.0 - weight) * pos.x;
		currentFloor.y = weight * floorWall.y + (1.0 - weight) * pos.y;
		floorTex.x = (int)(currentFloor.x * TEX_WIDTH) % TEX_WIDTH;
		floorTex.y = (int)(currentFloor.y * TEX_HEIGHT) % TEX_HEIGHT;
		buffer[y][x] = tiles[5][floorTex.y][floorTex.x];
		buffer[SCREEN_HEIGHT - y][x] = tiles[4][floorTex.y][floorTex.x];
	}
}

/**
 * renderBGFlat - draws ceiling and floor for flat maze
 * Return: void
 */
void renderBGFlat(void)
{
	SDL_Rect ceiling;
	SDL_Rect floor;
	int width;
	int height;

	SDL_GetWindowSize(window, &width, &height);

	ceiling.x = 0;
	ceiling.y = 0;
	ceiling.w = width;
	ceiling.h = height / 2;
	floor.x = 0;
	floor.y = height / 2;
	floor.w = width;
	floor.h = height / 2;

	SDL_SetRenderDrawColor(renderer, 0x59, 0x85, 0x94, 0xFF);
	SDL_RenderFillRect(renderer, &ceiling);
	SDL_SetRenderDrawColor(renderer, 0x1E, 0x29, 0x34, 0xFF);
	SDL_RenderFillRect(renderer, &floor);
}
