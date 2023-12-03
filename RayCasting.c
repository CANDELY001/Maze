#include "Maze.h"
/**
 * raycaster - casts rays and renders walls, floor, and ceiling
 * @maze: 2D array defining maze map
 * @textured: True if user enabled textures, otherwise False
 * Return: void
 */
void raycaster(int *maze, bool textured)
{
	double cameraX, distToWall;
	point_t rayPos, rayDir, posToNext, distToNext;
	SDL_Point map, step;
	int hit, side, x;

	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		cameraX = 2 * x / (double)(SCREEN_WIDTH) - 1;
		rayPos.x = pos.x, rayPos.y = pos.y, rayDir.x = dir.x + plane.x * cameraX;
		rayDir.y = dir.y + plane.y * cameraX;
		map.x = (int)(rayPos.x), map.y = (int)(rayPos.y);
		distToNext.x = sqrt(1 + (pow(rayDir.y, 2)) / pow(rayDir.x, 2));
		distToNext.y = sqrt(1 + (pow(rayDir.x, 2)) / pow(rayDir.y, 2));
		if (rayDir.x < 0)
		{
			step.x = -1, posToNext.x = (rayPos.x - map.x) * distToNext.x;
		} else
			step.x = 1, posToNext.x = (map.x + 1.0 - rayPos.x) * distToNext.x;
		if (rayDir.y < 0)
		{
			step.y = -1, posToNext.y = (rayPos.y - map.y) * distToNext.y;
		} else
			step.y = 1, posToNext.y = (map.y + 1.0 - rayPos.y) * distToNext.y;
		hit = 0;
		while (hit == 0)
		{
			if (posToNext.x < posToNext.y)
				posToNext.x += distToNext.x, map.x += step.x, side = 0;
			else
				posToNext.y += distToNext.y, map.y += step.y, side = 1;
			if (*((int *)maze + map.x * MAP_WIDTH + map.y) > 0)
				hit = 1;
		}
		if (side == 0)
			distToWall = (map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
		else
			distToWall = (map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
		renderWalls(maze, map, rayPos, rayDir, distToWall, x, side, textured);
	}
	updateRenderer(textured);
}
