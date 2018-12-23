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
#define FPS 50
#define MAP_WIDTH 600
#define MAP_HEIGHT 700
#define radius 15
#define house 70
#define numberofBullets 5
#define numberofWalls 36
#define numberofMaps 3

#ifdef main
#undef main
#endif

//void read_map_array(Wall *walls) {
//    int r = rand() % 2;
//    int i1 = rand() % (MAP_WIDTH / house) + 1;
//    int j1 = rand() % (MAP_HEIGHT / house) + 1;
//    int i2, j2;
//    if (r) {
//        i2 = rand() % (MAP_WIDTH / house + 1);
//        j2 = j1;
//    }
//    else if (i1){
//        j2 = rand() % (MAP_HEIGHT / house + 1);
//        i2 = i1;
//    }
//
//    walls->x1 = i1 * house;
//    walls->y1 = j1 * house;
//    walls->x2 = i2 * house;
//    walls->y2 = j2 * house;
//}


void read_map_file(Map* map, char* file_path){
    FILE *file1 = fopen(file_path, "r");
    int n, x1, y1, x2, y2;
    for (int j = 0; j < numberofWalls; j++) {
        fscanf(file1, "%d\n", &n);
        for (int i = 0; i < n; i++) {
            fscanf(file1, "%d %d %d %d\n", &x1, &y1, &x2, &y2);
            ((map + j)->walls + i)->x1 = x1 * house;
            ((map + j)->walls + i)->y1 = y1 * house;
            ((map + j)->walls + i)->x2 = x2 * house;
            ((map + j)->walls + i)->y2 = y2 * house;
        }
    }
    fclose(file1);
}


int main(int argc, char *argv[]) {
    Bullet *bullet = malloc(sizeof(Bullet) * numberofBullets);
    Tank *tank_1 = malloc(sizeof(Tank) * 1);
    Map *map = malloc(sizeof(Map) * numberofMaps);
    Wall *walls = malloc(sizeof(Wall) * numberofWalls);

    srand(time(0));
    for (int j = 0; j < numberofWalls; j++) {
        tank_1->x = rand() % (MAP_WIDTH - 2 * radius) + radius;
        tank_1->y = rand() % (MAP_HEIGHT - 2 * radius) + radius;
        tank_1->r = rand() % 255;
        tank_1->g = rand() % 255;
        tank_1->b = rand() % 255;
        tank_1->angle = rand() % 360;

        bullet->boolian = false;
        tank_1->bullets = bullet;
        (map + j)->tanks = tank_1;
        (map + j)->walls = walls;

        read_map_file(map + j, "D:\\programming\\c\\University\\project\\project\\src\\map.txt");

        init_window();
        while (1) {
            handle_events(map);
            for (int i = 0; i < numberofWalls; i++) draw_walls((map + j)->walls + i);
            for (int i = 0; i < numberofBullets; i++) draw_bullet((map + j)->tanks->bullets + i);
            draw_tank((map + j)->tanks);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000 / FPS);
        }
    }
    quit_window();
}