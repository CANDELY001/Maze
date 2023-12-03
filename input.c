#include "Maze.h"

/* Function prototypes*/
void updatePosition1(int *maze, double moveSpeed, const uint8_t *keystate);
void updatePosition2(int *maze, double moveSpeed, const uint8_t *keystate);
void rotateDirection(double rotateSpeed, const uint8_t *keystate);
/**
 * input - checks user input for movement
 * @maze: 2D array defining maze map
 * Return: void
 */
void input(int *maze)
{
	const uint8_t *keystate;
	double oldTime, frameTime;
	double moveSpeed, rotateSpeed;

	keystate = SDL_GetKeyboardState(NULL);
	oldTime = time, time = SDL_GetTicks();
	frameTime = (time - oldTime) / 1000.0;
	moveSpeed = frameTime * 5.0;
	rotateSpeed = frameTime * 3.0;

	updatePosition1(maze, moveSpeed, keystate);
	rotateDirection(rotateSpeed, keystate);
}
/**
 * rotateDirection - rotates player direction based on user input
 * @rotateSpeed: speed of rotation
 * @keystate: variabe parameter
 * Return: void
 */
void rotateDirection(double rotateSpeed, const uint8_t *keystate)
{
	double oldDirX, oldPlaneX;

	if (keystate[SDL_SCANCODE_D])
	{
		oldDirX = dir.x;
		dir.x = dir.x * cos(rotateSpeed) - dir.y * sin(rotateSpeed);
		dir.y = oldDirX * sin(rotateSpeed) + dir.y * cos(rotateSpeed);
		oldPlaneX = plane.x;
		plane.x = plane.x * cos(rotateSpeed) - plane.y * sin(rotateSpeed);
		plane.y = oldPlaneX * sin(rotateSpeed) + plane.y * cos(rotateSpeed);
	}
	if (keystate[SDL_SCANCODE_A])
	{
		oldDirX = dir.x;
		dir.x = dir.x * cos(-rotateSpeed) - dir.y * sin(-rotateSpeed);
		dir.y = oldDirX * sin(-rotateSpeed) + dir.y * cos(-rotateSpeed);
		oldPlaneX = plane.x;
		plane.x = plane.x * cos(-rotateSpeed) - plane.y * sin(-rotateSpeed);
		plane.y = oldPlaneX * sin(-rotateSpeed) + plane.y * cos(-rotateSpeed);
	}
}
/**
 * updatePosition1 - updates player position based on user input
 * @maze: 2D array defining maze map
 * @moveSpeed: speed of movement
 * @keystate: variabe parameter
 * Return: void
 */
void updatePosition1(int *maze, double moveSpeed, const uint8_t *keystate)
{
	if (keystate[SDL_SCANCODE_UP])
	{
		if (!*((int *)maze + (int)(pos.x + dir.x * moveSpeed)
					* MAP_WIDTH + (int)pos.y))
			pos.x += dir.x * moveSpeed;
		if (!*((int *)maze + (int)pos.x * MAP_WIDTH +
					(int)(pos.y + dir.y * moveSpeed)))
			pos.y += dir.y * moveSpeed;
	}
	else if (keystate[SDL_SCANCODE_DOWN])
	{
		if (!*((int *)maze + (int)(pos.x - dir.x * moveSpeed)
					* MAP_WIDTH + (int)(pos.y)))
			pos.x -= dir.x * moveSpeed;
		if (!*((int *)maze + (int)(pos.x) * MAP_WIDTH +
					(int)(pos.y - dir.y * moveSpeed)))
			pos.y -= dir.y * moveSpeed;
	}
	else
	{
		updatePosition2(maze, moveSpeed);
	}
}
/**
 * updatePosition2 - updates player position based on user input
 * @maze: 2D array defining maze map
 * @moveSpeed: speed of movement
 * @keystate: variabe parameter
 * Return: void
 */
void updatePosition2(int *maze, double moveSpeed, const uint8_t *keystate)
{
	if (keystate[SDL_SCANCODE_LEFT])
	{
		if (!*((int *)maze + (int)(pos.x - plane.x * moveSpeed)
					* MAP_WIDTH + (int)(pos.y)))
			pos.x -= plane.x * moveSpeed;
		if (!*((int *)maze + (int)(pos.x) * MAP_WIDTH
					+ (int)(pos.y - plane.y * moveSpeed)))
			pos.y -= plane.y * moveSpeed;
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		if (!*((int *)maze + (int)(pos.x + plane.x * moveSpeed)
					* MAP_WIDTH + (int)(pos.y)))
			pos.x += plane.x * moveSpeed;
		if (!*((int *)maze + (int)(pos.x) * MAP_WIDTH
					+ (int)(pos.y + plane.y * moveSpeed)))
			pos.y += plane.y * moveSpeed;
	}
}

/**
 * quit - checks if user quits
 * Return: True if user quits, else False
 */
bool quit(void)
{
	SDL_Event event;
	bool quit;
	uint32_t windowFlags;

	quit = false;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
			quit = true;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			quit = true;
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
		{
			windowFlags = SDL_GetWindowFlags(window);

			if (windowFlags & SDL_WINDOW_FULLSCREEN)
				SDL_SetWindowFullscreen(window, SDL_FALSE);
			else
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		}
	}
	return (quit);
}
