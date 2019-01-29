#include "structs.h"

bool nextGame_flag = false;

void move_tank(Tank *tank, Map *map) {
    int tempx = tank->x;
    int tempy = tank->y;
    if (state[tank->up] && !state[tank->down]) {
        if (movement_collids_walls(tank, map, 1)) tank->y += step * sin(tank->angle);
        if (movement_collids_walls(tank, map, 0)) tank->x += step * cos(tank->angle);
    }
    if (state[tank->down] && !state[tank->up]) {
        if (movement_collids_walls(tank, map, 1)) tank->y -= step * sin(tank->angle);
        if (movement_collids_walls(tank, map, 0)) tank->x -= step * cos(tank->angle);
    }
    if (tank->x < house / 2 + radius) {
        tank->x = house / 2 + radius;
        tank->y = tempy;
    }
    if (tank->y < house / 2 + radius) {
        tank->x = tempx;
        tank->y = house / 2 + radius;
    }
    if (tank->x > MAP_HEIGHT - house / 2 - radius) {
        tank->x = MAP_HEIGHT - house / 2 - radius;
        tank->y = tempy;
    }
    if (tank->y > MAP_HEIGHT - house / 2 - radius) {
        tank->x = tempx;
        tank->y = MAP_HEIGHT - house / 2 - radius;
    }
}

void turn_tank(Tank *tank, Map *map) {
    if (state[tank->right] && !state[tank->left] && turning_collids_walls(tank, map)) tank->angle += degree;
    if (state[tank->left] && !state[tank->right] && turning_collids_walls(tank, map)) tank->angle -= degree;
}

void move_bullet(Bullet *bullet) {
    if ((bullet->n) < 3 * distanceofBullets / 2 && !bullet->boolian) (bullet->n)++;
    if (bullet->boolian && bullet->x != -100) {
        bullet->x += step_bullet * cos(bullet->angle);
        bullet->y += step_bullet * sin(bullet->angle);
        (bullet->n)++;
        if (bullet->n == distanceofBullets) {
            bullet->boolian = false;
        }
    }
}

void fire(Tank *tank) {
    static bool flag[] = {true, true};
    static int i[] = {0, 0};
    for (int k = 0; k < numberofTanks; k++) {
        if (((tank + k)->bullets + numberofBullets - 1)->n == 3 * distanceofBullets / 2 || start_flag) {
            if (start_flag && k == numberofTanks - 1) start_flag = false;
            i[k] = 0;
            for (int j = 0; j < numberofBullets; j++) {
                ((tank + k)->bullets + j)->boolian = true;
                ((tank + k)->bullets + j)->x = -100;
                ((tank + k)->bullets + j)->y = -100;
            }
            ((tank + k)->bullets + numberofBullets - 1)->n = 0;
            printf("hi\n");
        }

        if (state[(tank + k)->shoot] && (tank + k)->boolian) {
            if (i[k] < numberofBullets && (tank->bullets + i[k])->boolian && flag[k]) {
                ((tank + k)->bullets + i[k])->x =
                        (tank + k)->x + (shooter + radius_shooter + radius_bullet) * cos((tank + k)->angle);
                ((tank + k)->bullets + i[k])->y =
                        (tank + k)->y + (shooter + radius_shooter + radius_bullet) * sin((tank + k)->angle);
                ((tank + k)->bullets + i[k])->angle = (tank + k)->angle;
                ((tank + k)->bullets + i[k])->n = 0;
                flag[k] = false;
                i[k]++;
                printf("%d\n", i[k]);
            }
            static int n[numberofTanks] = {0};
            n[k]++;
            if (n[k] == 10) {
                flag[k] = true;
                n[k] = 0;
            }
        }
    }
}
