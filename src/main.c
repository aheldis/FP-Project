#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <time.h>

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
#define house 28
#define numberofBullets 5

//void read_map_array(Map* map, Wall* saved_walls) {
//    for(int i=1; i<25; i++){
//        for(int j=1; j<25 ;j++){
//            r=rand()%4;
//            saved_walls->x1 = i * house;
//            saved_walls->y1 = j * house;
//            saved_walls->x2 = i * house;
//            saved_walls->y2 = j * house;
//            setcolor(3);
//            rectangle(i*100,j*100,i*100+100,j*100+100);
//            a[i][j] = 1;
//        }
//    }
//}


#ifdef main
#undef main
#endif

int main(int argc, char *argv[]) {
    Bullet *bullet = malloc(sizeof(Bullet) * 1);
    Tank *tank_1 = malloc(sizeof(Tank) * 1);
    Map *map = malloc(sizeof(Map) * 1);

    srand (time (0));
    tank_1->x = rand() % (MAP_WIDTH - 2 * radius) + radius ;
    tank_1->y = rand() % (MAP_HEIGHT - 2 * radius) + radius;
    tank_1->r = rand() % 255;
    tank_1->g = rand() % 255;
    tank_1->b = rand() % 255;
    tank_1->angle = rand() % 360;

    bullet->boolian = false;
    tank_1->bullets = bullet;
    map->tanks = tank_1;

    init_window();
    while(1) {
        handle_events(map);
        draw_walls(map->walls);
        draw_bullet(map->tanks->bullets);
        draw_tank(map->tanks);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    quit_window();
}