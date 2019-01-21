#include "structs.h"

static bool flag = true;

void move_tank(Tank *tank) {
    int tempx = tank->x;
    int tempy = tank->y;
    if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
        tank->y -= step * sin(tank->angle);
        tank->x += step * cos(tank->angle);
    }
    if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP]) {
        tank->y += step * sin(tank->angle);
        tank->x -= step * cos(tank->angle);
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
    if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) tank->angle -= degree;
    if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) tank->angle += degree;
}

void move_bullet(Bullet *bullet) {
    static int n = 0;
    bullet->x += step_bullet * cos(bullet->angle);
    bullet->y -= step_bullet * sin(bullet->angle);
    bullet->n++;
    if (bullet->n == distanceofBullets) {
        bullet->boolian = false;
        bullet->n = 0;
    }
}

void fire(Tank *tank) {
    flag = false;
    static int n = 0;
    n++;
    if (n == 6) {
        flag = true;
        n = 0;
    }
    if (state[SDL_SCANCODE_M] && flag) {
        static int i = 0;
        if (i < numberofBullets) {
            (tank->bullets + i)->boolian = true;
            (tank->bullets + i)->x = tank->x + shooter * cos(tank->angle);
            (tank->bullets + i)->y = tank->y - shooter * sin(tank->angle);
            (tank->bullets + i)->angle = tank->angle;
            (tank->bullets + i)->n = 0;
            i++;
            printf ("%d\n", i);
        }
    }
}
