#include "structs.h"

void move_tank(Tank *tank) {
    int tempx = tank->x;
    int tempy = tank->y;
    if (state[tank->up] && !state[tank->down]) {
        tank->y -= step * sin(tank->angle);
        tank->x += step * cos(tank->angle);
    }
    if (state[tank->down] && !state[tank->up]) {
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
    if (state[tank->right] && !state[tank->left]) tank->angle -= degree;
    if (state[tank->left] && !state[tank->right]) tank->angle += degree;
}

void move_bullet(Bullet *bullet) {
    if ((bullet->n) < 2 * distanceofBullets) (bullet->n)++;
    if (bullet->boolian) {
        bullet->x += step_bullet * cos(bullet->angle);
        bullet->y -= step_bullet * sin(bullet->angle);
        if (bullet->n == distanceofBullets) {
            bullet->boolian = false;
        }
    }
}

void fire(Tank *tank) {
    static bool flag[] = {true, true};
    static int i[] = {0, 0};
    for (int k = 0; k < 2; k++) {
        if (((tank + k)->bullets + numberofBullets - 1)->n == 2 * distanceofBullets) {
            i[k] = 0;
            for (int j = 0; j < numberofBullets; j++) {
                ((tank + k)->bullets + j)->boolian = true;
                ((tank + k)->bullets + j)->x = -100;
                ((tank + k)->bullets + j)->y = -100;
            }
            ((tank + k)->bullets + numberofBullets - 1)->n = 0;
            printf("hi\n");
        }
    }
    if (state[tank->shoot]) {
        if (i[0] < numberofBullets && (tank->bullets + i[0])->boolian && flag[0]) {
            (tank->bullets + i[0])->x = tank->x + shooter * cos(tank->angle);
            (tank->bullets + i[0])->y = tank->y - shooter * sin(tank->angle);
            (tank->bullets + i[0])->angle = tank->angle;
            (tank->bullets + i[0])->n = 0;
            flag[0] = false;
            i[0]++;
            printf ("%d\n", i[0]);
        }
        static int n = 0;
        n++;
        if (n == 10) {
            flag[0] = true;
            n = 0;
        }
    }

    if (state[(tank + 1)->shoot]) {
        if (i[1] < numberofBullets && (tank->bullets + i[1])->boolian && flag[1]) {
            ((tank + 1)->bullets + i[1])->x = (tank + 1)->x + shooter * cos((tank + 1)->angle);
            ((tank + 1)->bullets + i[1])->y = (tank + 1)->y - shooter * sin((tank + 1)->angle);
            ((tank + 1)->bullets + i[1])->angle = (tank + 1)->angle;
            ((tank + 1)->bullets + i[1])->n = 0;
            flag[1] = false;
            i[1]++;
            printf ("%d\n", i[1]);
        }
        static int n = 0;
        n++;
        if (n == 10) {
            flag[1] = true;
            n = 0;
        }
    }
}
