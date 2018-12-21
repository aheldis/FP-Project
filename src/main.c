#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <time.h>
#include <stdbool.h>

#include "view.h"
#include "physics.h"
#include "logic.h"
#include "structs.h"

#define red 255
#define green 255
#define blue 255
#define a 255
#define FPS 25
#define MAP_WIDTH 700
#define MAP_HEIGHT 700
#define radius 15

void read_map_array(Map* map, Wall* saved_walls);
void read_map_file(Map* map, char* file_path);

#ifdef main
#undef main
#endif

int main(int argc, char *argv[]) {
    Bullet *bullet = malloc(sizeof(Bullet) * 1);
    Tank *tank_1 = malloc(sizeof(Tank) * 1);
    Map *map = malloc(sizeof(Map) * 1);

    srand (time (0));
    tank_1->x = rand() % (MAP_WIDTH - 2 * radius) + radius ;
    srand (time (0));
    tank_1->y = rand() % (MAP_HEIGHT - 2 * radius) + radius;
    srand (time (0));
    tank_1->r = rand() % 255;
    srand (time (0));
    tank_1->g = rand() % 255;
    srand (time (0));
    tank_1->b = rand() % 255;
    srand (time (0));
    tank_1->angle = rand() % 360;

    bullet->boolian = false;
    tank_1->bullets = bullet;
    map->tanks = tank_1;

    init_window();
    while(1) {8
        SDL_SetRenderDrawColor(renderer, red, green, blue, a);
        SDL_RenderClear(renderer);
        draw_tank(map->tanks);
        handle_events(map);
        draw_bullet(map->tanks->bullets);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    quit_window();
}