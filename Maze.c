#include "Maze.h"

/* global variables */
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
uint32_t tiles[TEX_COUNT][TEX_HEIGHT][TEX_WIDTH];
point_t pos;
point_t dir;
point_t plane;
double time;

/**
 * main - renders maze
 * @argc: number of arguments
 * @argv: pointer to array of arguments
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
    int *maze;
    char *mapName;
    bool textured;

    pos.x = 1;
    pos.y = 12;
    dir.x = 1;
    dir.y = -0.66;
    plane.x = 0;
    plane.y = 0.66;
    time = 0;

    mapName = "\0";
    textured = true;
    if (argc == 3)
    {
        if (strcmp(argv[2], "no_tex") == 0)
            textured = false;
        mapName = argv[1];
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "no_tex") == 0)
        {
            mapName = "maps/map_0";
            textured = false;
        }
        else
            mapName = argv[1];
    }
    else if (argc == 1)
        mapName = "maps/map_0";
    if (!initSDL())
        return (1);

    maze = NULL;
    maze = parseMap(mapName, maze);
    if (maze == NULL)
        return (1);
    if (textured)
        loadTextures(mapName);
    while (!quit())
    {
        if (!textured)
            renderBGFlat();
        raycaster(maze, textured);
        input(maze);
    }
    closeSDL();
    free(maze);
    return (0);
}
