#include "structs.h"

bool start_flag = false;

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
    if (tank->x > MAP_WIDTH - house / 2 - house - radius) {
        tank->x = MAP_WIDTH - house / 2 - house - radius;
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

void move_shard(Shard *shard, Map *map) {
    if (shard_collids_walls(shard, map)) {
        shard->x += step_bullet * cos(shard->angle);
        shard->y += step_bullet * sin(shard->angle);
    }
    (shard->n)++;
    if (shard->n == distanceofShards) {
        shard->n = 0;
        shard->boolian = false;
    }
}

void fire(Tank *tank, Shard *shard, Mine *mine) {
    static bool flag[] = {true, true};
    static int i[] = {0, 0};
    static int n[numberofTanks] = {0};
    for (int k = 0; k < numberofTanks; k++) {
        if (((tank + k)->bullets + numberofBullets - 1)->n == 3 * distanceofBullets / 2 || start_flag) {
            if (start_flag && k == numberofTanks - 1) start_flag = false;
            i[k] = 0;
            flag[k] = true;
            n[k] = 0;
            for (int j = 0; j < numberofBullets; j++) {
                ((tank + k)->bullets + j)->boolian = true;
                ((tank + k)->bullets + j)->x = -100;
                ((tank + k)->bullets + j)->y = -100;
            }
            ((tank + k)->bullets + numberofBullets - 1)->n = 0;
            printf("hi\n");
        }

        if (state[(tank + k)->shoot] && (tank + k)->boolian) {
            //printf("i have bug!\n");
            //fflush(stdout);
            if (i[k] < numberofBullets && (tank->bullets + i[k])->boolian && (flag[k] || (tank + k)->fragBomb == 1)) {
                if ((tank + k)->lazer == 1) {
                    (tank + k)->lazer = 2;
                    (tank + k)->lazerTime = 0;
                } else if ((tank + k)->mine == 1) {
                    (mine + k)->x =
                            (tank + k)->x - (shooter + radius_shooter + radius_mine + step) * cos((tank + k)->angle);
                    (mine + k)->y =
                            (tank + k)->y - (shooter + radius_shooter + radius_mine + step) * sin((tank + k)->angle);
                    (mine + k)->boolian = true;
                    (mine + k)->n = 0;
                    (mine + k)->r = (tank + k)->r;
                    (mine + k)->g = (tank + k)->g;
                    (mine + k)->b = (tank + k)->b;
                    (tank + k)->mine = 2;
                } else if ((tank + k)->lazer == 0 && (tank + k)->mine == 0 &&
                           (((tank + k)->item == false && (tank + k)->fragBomb == 0 && flag[k]) ||
                            ((tank + k)->item == true && (tank + k)->fragBomb == 1))) {
                    if ((tank + k)->fragBomb == 1) {
                        ((tank + k)->bullets + i[k])->rad = radius_fragBomb;
                        (tank + k)->fragBomb = 2;
                    }
                    ((tank + k)->bullets + i[k])->x =
                            (tank + k)->x +
                            (shooter + radius_shooter + ((tank + k)->bullets + i[k])->rad) * cos((tank + k)->angle);
                    ((tank + k)->bullets + i[k])->y =
                            (tank + k)->y +
                            (shooter + radius_shooter + ((tank + k)->bullets + i[k])->rad) * sin((tank + k)->angle);
                    ((tank + k)->bullets + i[k])->angle = (tank + k)->angle;
                    ((tank + k)->bullets + i[k])->n = 0;
                    flag[k] = false;
                    if ((tank + k)->fragBomb == 0) i[k]++;
                    printf("%d\n", i[k]);
                } else if ((tank + k)->item == true && (tank + k)->fragBomb == 2) {
                    for (int j = k * numberofShards; j < (k + 1) * numberofShards; j++) {
                        (shard + j)->x = ((tank + k)->bullets + i[k])->x;
                        (shard + j)->y = ((tank + k)->bullets + i[k])->y;
                        (shard + j)->n = 0;
                        (shard + j)->angle = (rand() % 360) * (M_PI / 180);
                        (shard + j)->boolian = true;
                    }
                    ((tank + k)->bullets + i[k])->x = -100;
                    ((tank + k)->bullets + i[k])->y = -100;
                    ((tank + k)->bullets + i[k])->rad = radius_bullet;
                    (tank + k)->fragBomb = 0;
                    (tank + k)->item = false;
                    flag[k] = false;
                }
            }
            n[k]++;
            if (n[k] == 10) {
                flag[k] = true;
                n[k] = 0;
            }
        }
    }
}
