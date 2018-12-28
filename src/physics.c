#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "structs.h"
#include "view.h"

#define step 6
#define degree 0.4
#define MAP_WIDTH 700
#define MAP_HEIGHT 700
#define radius 15
#define shooter 20
#define numberofBullets 5
#define distanceofBullets 200

void move_tank(Tank *tank) {
    int tempx = tank->x;
    int tempy = tank->y;
    switch (keycode){
        case SDLK_UP:
            tank->y -= step * sin(tank->angle);
            tank->x += step * cos(tank->angle);
            break;
        case SDLK_DOWN:
            tank->y += step * sin(tank->angle);
            tank->x -= step * cos(tank->angle);
            break;
    }
    if (tank->x < radius) {
        tank->x = radius;
        tank->y = tempy;
    }
    if (tank->y < radius) {
        tank->x = tempx;
        tank->y = radius;
    }
    if (tank->x > MAP_WIDTH - radius) {
        tank->x = MAP_WIDTH - radius;
        tank->y = tempy;
    }
    if (tank->y > MAP_HEIGHT - radius) {
        tank->x = tempx;
        tank->y = MAP_HEIGHT - radius;
    }
}

void turn_tank(Tank *tank) {
    switch (keycode){
        case SDLK_RIGHT:
            tank->angle -= degree;
            break;
        case SDLK_LEFT:
            tank->angle += degree;
            break;
    }
}

void move_bullet(Bullet *bullet) {
    static int n = 0;
    bullet->x += step * cos(bullet->angle);
    bullet->y -= step * sin(bullet->angle);
    n++;
    if (n == distanceofBullets) {
        bullet->boolian = false;
        n = 0;
    }
}

void fire(Tank *tank) {
    if (keycode == SDLK_m) {
        static int i = 0;
        if (i < numberofBullets) {
            (tank->bullets + i)->boolian = true;
            (tank->bullets + i)->x = tank->x + shooter * cos(tank->angle);
            (tank->bullets + i)->y = tank->y - shooter * sin(tank->angle);
            (tank->bullets + i)->angle = tank->angle;
            i++;
        }
    }
}
