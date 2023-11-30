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
void renderWalls(int *maze, SDL_Point map, point_t rayPos, point_t rayDir, double distToWall, int x, int side, bool textured)
{
    int sliceHeight;
    int drawStart;
    int drawEnd;
    int tileIndex;
    double wallX;
    SDL_Point tex;
    uint32_t color;
    int width;
    int height;
    int y;

    if (!textured)
    {
        SDL_GetWindowSize(window, &width, &height);

        sliceHeight = (int)(height / distToWall);

        drawStart = -sliceHeight / 2 + height / 2;
        if (drawStart < 0)
            drawStart = 0;
        drawEnd = sliceHeight / 2 + height / 2;
        if (drawEnd >= height)
            drawEnd = height - 1;
        if (side == 0)
            SDL_SetRenderDrawColor(renderer, 0xF7, 0xF7, 0xF7, 0xFF);
        else if (side == 1)
            SDL_SetRenderDrawColor(renderer, 0xDE, 0xDE, 0xDE, 0xFF);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
    else
    {
        sliceHeight = (int)(SCREEN_HEIGHT / distToWall);

        drawStart = -sliceHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;

        drawEnd = sliceHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        wallX = 0;
        if (side == 0)
            wallX = rayPos.y + distToWall * rayDir.y;
        else if (side == 1)
            wallX = rayPos.x + distToWall * rayDir.x;

        tileIndex = *((int *)maze + map.x * MAP_WIDTH + map.y) - 1;

        wallX -= floor(wallX);

        tex.x = (int)(wallX * (double)TEX_WIDTH);
        if (side == 0 && rayDir.x > 0)
            tex.x = TEX_WIDTH - tex.x - 1;
        if (side == 1 && rayDir.y < 0)
            tex.x = TEX_WIDTH - tex.x - 1;

        for (y = drawStart; y < drawEnd; y++)
        {
            tex.y = ((((y << 1) - SCREEN_HEIGHT + sliceHeight) <<
                  (int)log2(TEX_HEIGHT)) / sliceHeight) >> 1;

            color = tiles[tileIndex][tex.x][tex.y];

            if (side == 1)
                color = (color >> 1) & 8355711;

            buffer[y][x] = color;
        }
        renderBGTex(map, rayDir, distToWall, wallX, drawEnd, x, side);
    }
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
void renderBGTex(SDL_Point map, point_t rayDir, double distToWall, double wallX, int drawEnd, int x, int side)
{
    point_t floorWall;
    point_t currentFloor;
    SDL_Point floorTex;
    double weight;
    double currentDist;
    int y;

    if (side == 0 && rayDir.x > 0)
    {
        floorWall.x = map.x;
        floorWall.y = map.y + wallX;
    }
    else if (side == 0 && rayDir.x < 0)
    {
        floorWall.x = map.x + 1.0;
        floorWall.y = map.y + wallX;
    }
    else if (side == 1 && rayDir.y > 0)
    {
        floorWall.x = map.x + wallX;
        floorWall.y = map.y;
    }
    else
    {
        floorWall.x = map.x + wallX;
        floorWall.y = map.y + 1.0;
    }
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
