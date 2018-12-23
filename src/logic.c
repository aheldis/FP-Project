#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2_gfxPrimitives.h>

#include "structs.h"

#define shooter 25
#define thick 3
#define numberofWalls 36

bool movement_collids_walls(Tank *tank, Map *map) {
    for (int i = 0; i < numberofWalls; i++) {
        if (abs(tank->x + shooter * cos(180 / M_PI * tank->angle) - (map->walls + i)->x1) <= thick && !((map->walls + i)->x1 - (map->walls + i)->x2))   return false;
        if (abs(tank->y - shooter * sin(180 / M_PI * tank->angle) - (map->walls + i)->y1) <= thick && !((map->walls + i)->y1 - (map->walls + i)->y2))   return false;
    }
    return true;
}