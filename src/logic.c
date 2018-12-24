#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2_gfxPrimitives.h>

#include "structs.h"
#include "view.h"

#define shooter 20
#define radius 15
#define thick 4
#define numberofWalls 36
#define step 5

bool movement_collids_walls(Tank *tank, Map *map) {
    for (int i = 0; i < numberofWalls; i++) {
        if ((abs(abs(tank->x + (shooter + step) * cos(180 / M_PI * tank->angle) - (map->walls + i)->x1) - 5) <= thick) &&
            ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
            (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) < abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + 2))
            return false;
        if ((abs(abs(tank->y - (shooter + step) * sin(180 / M_PI * tank->angle) - (map->walls + i)->y1) - 5) <= thick) &&
            ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
            (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) < abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + 2))
            return false;

        int sign_x = (cos(180 / M_PI * tank->angle) >= 0);
        int sign_y = (sin(180 / M_PI * tank->angle) >= 0);
        if (keycode == SDLK_DOWN) {
            sign_x = !sign_x;
            sign_y = !sign_y;
        }
        if ((((abs(tank->x + radius - (map->walls + i)->x1) <= thick) && (sign_x)) ||
            ((abs(tank->x - radius - (map->walls + i)->x1) <= thick) && (!sign_x))) &&
            ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
            (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <
            abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + 2))
            return false;
        if ((((abs(tank->y - radius - (map->walls + i)->y1) <= thick) && (sign_y)) ||
            ((abs(tank->y + radius - (map->walls + i)->y1) <= thick) && (!sign_y))) &&
            ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
            (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <
            abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + 2))
            return false;
    }
    return true;
}