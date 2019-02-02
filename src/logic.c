#include "structs.h"

int remaining;

short vertex(int x, int y) {
    return (x - house / 2) / house + (y - house / 2) / house * numberofColumns;
}

short left(short vert) {
    return vert % numberofColumns + vert / numberofColumns * (numberofColumns + 1);
}

short right(short vert) {
    return vert % numberofColumns + vert / numberofColumns * (numberofColumns + 1) + 1;
}

short up(short vert) {
    return vert + numberofRows * (numberofColumns + 1);
}

short down(short vert) {
    return vert + numberofRows * (numberofColumns + 1) + numberofColumns;
}

bool movement_collids_walls(Tank *tank, Map *map, int flag) {
    if (tank->lazer == 2) return false;
    //////the house where tank is in it
    short vert = vertex(tank->x, tank->y);
    int max_x, max_y;
    int sign_x = (cos(tank->angle) > 0 ? 1 : -1);
    int sign_y = (sin(tank->angle) > 0 ? 1 : -1);
    if (((shooter + step) * cos(tank->angle) + radius_shooter * sign_x) * sign_x >
        radius_circle + step * cos(tank->angle))
        max_x = (shooter + step) * cos(tank->angle) + radius_shooter * sign_x;
    else max_x = radius_circle * sign_x + step * cos(tank->angle);
    if (((shooter + step) * sin(tank->angle) + radius_shooter * sign_y) * sign_y >
        radius_circle + step * sin(tank->angle))
        max_y = (shooter + step) * sin(tank->angle) + radius_shooter * sign_y;
    else max_y = radius_circle * sign_y + step * sin(tank->angle);

    for (int j = 0; j < 4; j++) {
        short l = left(vert);
        short r = right(vert);
        short u = up(vert);
        short d = down(vert);
        int b[] = {r, d, l, u};
        int i = b[j];
        if ((map->walls + i)->boolian) {
            ////////////////////collision
            if (flag == 0)
                if ((((abs(tank->x + max_x - (map->walls + i)->x1) <= thick) && state[tank->up]) ||
                     ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <=
                       thick) && state[tank->down])) &&
                    (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                     (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                      abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick)))
                    return false;
            if (flag == 1)
                if ((((abs(tank->y + max_y - (map->walls + i)->y1) <= thick) && state[tank->up]) ||
                     ((abs(tank->y - radius_circle * sign_y - step * sin(tank->angle) - (map->walls + i)->y1) <=
                       thick) && state[tank->down])) &&
                    ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                    (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                     abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick))
                    return false;
        }
    }
    short vertl = vertex(tank->x + (max_x < 0) * max_x - (max_x > 0) * (radius_circle + step * cos(tank->angle)),
                         tank->y);
    short vertr = vertex(tank->x + (max_x > 0) * max_x + (max_x < 0) * (radius_circle + step * cos(tank->angle)),
                         tank->y);
    short vertu = vertex(tank->x,
                         tank->y + (max_y < 0) * max_y - (max_y > 0) * (radius_circle + step * sin(tank->angle)));
    short vertd = vertex(tank->x,
                         tank->y + (max_y > 0) * max_y + (max_y < 0) * (radius_circle + step * sin(tank->angle)));
    int c[] = {vertl, vertr, vertu, vertd};
    for (int i = 0; i < 4; i++) {
        short verte = vert;
        if (verte != c[i]) {
            verte = c[i];
            for (int j = 0; j < 4; j++) {
                short l = left(verte);
                short r = right(verte);
                short u = up(verte);
                short d = down(verte);
                int b[] = {r, d, l, u};
                int i = b[j];
                if ((map->walls + i)->boolian) {
                    ////////////////////collision
                    if (flag == 0)
                        if ((((abs(tank->x + max_x - (map->walls + i)->x1) <= thick) && (state[tank->up])) ||
                             ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <=
                               thick) && state[tank->down])) &&
                            (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                             (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                              abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick)))
                            return false;
                    if (flag == 1)
                        if ((((abs(tank->y + max_y - (map->walls + i)->y1) <= thick) && (state[tank->up])) ||
                             ((abs(tank->y - radius_circle * sign_y - step * sin(tank->angle) - (map->walls + i)->y1) <=
                               thick) && state[tank->down])) &&
                            ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                            (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                             abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick))
                            return false;
                }
            }
        }
    }

    if (vertu != vertd) {
        short l = 0, r = 0;
        if (vert - vertu == 9 || vert - vertu == 7) vertu = vert - 8;
        if (vert % numberofColumns != 0) l = down(vertu - 1);
        if (vert % numberofColumns != numberofColumns - 1) r = down(vertu + 1);
        int b[] = {l, r};
        for (int j = 0; j < 2; j++) {
            int i = b[j];
            if (i && (map->walls + i)->boolian && !(map->walls + down(vertu))->boolian) {
                if ((pow((map->walls + i)->x1 -
                         (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     min(pow((map->walls + i)->y1 + thick -
                             (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2),
                         pow((map->walls + i)->y1 - thick -
                             (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2)) <=
                     (max_x + thick) * (max_x + thick) * state[tank->up] +
                     (radius_circle + thick / 2) * (radius_circle + thick / 2) * state[tank->down]) ||
                    (pow((map->walls + i)->x2 -
                         (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     min(pow((map->walls + i)->y1 + thick -
                             (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2),
                         pow((map->walls + i)->y1 - thick -
                             (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2)) <=
                     (max_x + thick) * (max_x + thick) * state[tank->up] +
                     (radius_circle + thick / 2) * (radius_circle + thick / 2) * state[tank->down])) {
                    if (flag == 0 && (((sign_x == 1 && i == r) || (sign_x == -1 && i == l)) && state[tank->up]) ||
                        (((sign_x == 1 && i == l) || (sign_x == -1 && i == r)) && state[tank->down]))
                        return false;
//                    if (flag == 1 && state[tank->up] &&
//                        (abs(tank->x + max_x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
//                         abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2) &&
//                        ((sign_y == 1 && tank->y + max_y >= (map->walls + i)->y1 - thick) ||
//                         (sign_y == -1 && tank->y + max_y <= (map->walls + i)->y1 + thick)))
//                        return false;
                }
            }
        }
    }
    if (vertl != vertr) {
        short u = 0, d = 0;
        if (vert - vertr == 7 || vertr - vert == 9) vertr = vert + 1;
        if (vert / numberofColumns != 0) u = left(vertr - numberofColumns);
        if (vert / numberofColumns != numberofColumns - 1) d = left(vertr + numberofColumns);
        int b[] = {u, d};
        for (int j = 0; j < 2; j++) {
            int i = b[j];
            if (i && (map->walls + i)->boolian && !(map->walls + left(vertr))->boolian)
                if ((pow((map->walls + i)->y1 -
                         (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     min(pow((map->walls + i)->x1 + thick -
                             (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2),
                         pow((map->walls + i)->x1 - thick -
                             (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2)) <=
                     (max_y + thick) * (max_y + thick) * state[tank->up] +
                     (radius_circle + thick) * (radius_circle + thick) * state[tank->down]) ||
                    (pow((map->walls + i)->y2 -
                         (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     min(pow((map->walls + i)->x1 + thick -
                             (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2),
                         pow((map->walls + i)->x1 - thick -
                             (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2)) <=
                     (max_y + thick) * (max_y + thick) * state[tank->up] +
                     (radius_circle + thick) * (radius_circle + thick) * state[tank->down])) {
                    if (flag == 1 && (((sign_y == 1 && i == d) || (sign_y == -1 && i == u)) && state[tank->up]) ||
                        (((sign_y == 1 && i == u) || (sign_y == -1 && i == d)) && state[tank->down]))
                        return false;
//                    if (flag == 0 && state[tank->up] &&
//                        (abs(tank->y + max_y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
//                         abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2) &&
//                        ((sign_x == 1 && tank->x + max_x >= (map->walls + i)->x1 - thick) ||
//                         (sign_x == -1 && tank->x + max_x <= (map->walls + i)->x1 + thick)))
//                        return false;
                }
        }
    }
    return true;
}

bool turning_collids_walls(Tank *tank, Map *map) {
    if (tank->lazer == 2) return false;
    //////the house where tank is in it
    short vert = vertex(tank->x, tank->y);
    double deg = (tank->angle + degree) * state[tank->right] + (tank->angle - degree) * state[tank->left];
    int max_x, max_y;
    int sign_x = (cos(deg) > 0 ? 1 : -1);
    int sign_y = (sin(deg) > 0 ? 1 : -1);
    if (((shooter + step) * cos(deg) + radius_shooter * sign_x) * sign_x >
        radius_circle + step * cos(deg))
        max_x = (shooter + step) * cos(deg) + radius_shooter * sign_x;
    else max_x = radius_circle * sign_x + step * cos(deg);
    if (((shooter + step) * sin(deg) + radius_shooter * sign_y) * sign_y >
        radius_circle + step * sin(deg))
        max_y = (shooter + step) * sin(deg) + radius_shooter * sign_y;
    else max_y = radius_circle * sign_y + step * sin(deg);
    short vertl = vertex(tank->x + (max_x < 0) * max_x - (max_x > 0) * (radius_circle + step * cos(deg)),
                         tank->y);
    short vertr = vertex(tank->x + (max_x > 0) * max_x + (max_x < 0) * (radius_circle + step * cos(deg)),
                         tank->y);
    short vertu = vertex(tank->x,
                         tank->y + (max_y < 0) * max_y - (max_y > 0) * (radius_circle + step * sin(deg)));
    short vertd = vertex(tank->x,
                         tank->y + (max_y > 0) * max_y + (max_y < 0) * (radius_circle + step * sin(deg)));
    for (int j = 0; j < 4; j++) {
        short l = left(vert);
        short r = right(vert);
        short u = up(vert);
        short d = down(vert);
        int b[] = {r, d, l, u};
        int i = b[j];
        ////////////////////rotation
        if ((map->walls + i)->boolian) {
            if ((((abs(
                    tank->x + radius_shooter * sign_x + shooter * cos(tank->angle + degree) - (map->walls + i)->x1) <=
                   thick) && (state[tank->right])) ||
                 ((abs(tank->x + radius_shooter * sign_x + shooter * cos(tank->angle - degree) -
                       (map->walls + i)->x1) <=
                   thick) && (state[tank->left]))) &&
                ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                 abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + radius_circle + thick)) {
                tank->x -= step * cos(tank->angle);
                //return false;
            }
            if ((((abs(
                    tank->y + radius_shooter * sign_y + shooter * sin(tank->angle + degree) - (map->walls + i)->y1) <=
                   thick) && (state[tank->right])) ||
                 ((abs(tank->y + radius_shooter * sign_y + shooter * sin(tank->angle - degree) -
                       (map->walls + i)->y1) <=
                   thick) && (state[tank->left]))) &&
                ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                 abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + radius_circle + thick)) {
                tank->y -= step * sin(tank->angle);
                //return false;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        int c[] = {vertl, vertr, vertu, vertd};
        short verte = vert;
        if (verte != c[i]) {
            verte = c[i];
            for (int j = 0; j < 4; j++) {
                short l = left(verte);
                short r = right(verte);
                short u = up(verte);
                short d = down(verte);
                int b[] = {r, d, l, u};
                int i = b[j];
                if ((map->walls + i)->boolian) {
                    //printf("hi");
                    ////////////////////rotation
                    if ((((abs(
                            tank->x + radius_shooter * sign_x + shooter * cos(tank->angle + degree) -
                            (map->walls + i)->x1) <= thick) && (state[tank->right])) ||
                         ((abs(tank->x + radius_shooter * sign_x + shooter * cos(tank->angle - degree) -
                               (map->walls + i)->x1) <= thick) && (state[tank->left]))) &&
                        ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                        (abs(tank->y + radius_shooter * sign_y + shooter * sin(deg) -
                             ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <
                         abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + shooter + radius_shooter + thick)) {
                        //tank->y -= step * sin(tank->angle);
                        tank->x -= step * cos(tank->angle);
                        //return false;
                    }
                    if ((((abs(
                            tank->y + radius_shooter * sign_y + shooter * sin(tank->angle + degree) -
                            (map->walls + i)->y1) <= thick) && (state[tank->right])) ||
                         ((abs(tank->y + radius_shooter * sign_y + shooter * sin(tank->angle - degree) -
                               (map->walls + i)->y1) <= thick) && (state[tank->left]))) &&
                        ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                        (abs(tank->x + radius_shooter * sign_x + shooter * cos(deg) -
                             ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <
                         abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + shooter + radius_shooter + thick)) {
                        tank->y -= step * sin(tank->angle);
                        //tank->x -= step * cos(tank->angle);
                        //return false;
                    }
                }
            }
        }
    }

    return true;
}

void bullet_collids_walls(Bullet *bullet, Map *map) {
    //////the house where bullet is in it
    short vert = vertex(bullet->x, bullet->y);
    int sign_x = (cos(bullet->angle) > 0 ? 1 : -1);
    int sign_y = (sin(bullet->angle) > 0 ? 1 : -1);
    for (int j = 0; j < 4; j++) {
        short l = left(vert);
        short r = right(vert);
        short u = up(vert);
        short d = down(vert);
        int b[] = {r, d, l, u};
        int i = b[j];
        if ((map->walls + i)->boolian) {
            ////////////////////collision
            if (((abs(bullet->x + bullet->rad * sign_x - (map->walls + i)->x1) <=
                  thick) ||
                 (abs(bullet->x + bullet->rad * sign_x + step_bullet * cos(bullet->angle) - (map->walls + i)->x1) <=
                  thick)) && ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                (abs(bullet->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                 abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + bullet->rad + thick * 2))
                bullet->angle = M_PI - bullet->angle;

            else if ((bullet->x + bullet->rad * sign_x >=
                      (map->walls + i)->x1 + thick ||
                      bullet->x + bullet->rad * sign_x + step_bullet * cos(bullet->angle) >=
                      (map->walls + i)->x1 + thick) &&
                     (bullet->x + bullet->rad * sign_x <= (map->walls + i)->x1 - thick) &&
                     ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                     (abs(bullet->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                      abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + bullet->rad + thick * 2))
                bullet->angle = M_PI - bullet->angle;

            if ((abs(bullet->y + bullet->rad * sign_y - (map->walls + i)->y1) <=
                 thick ||
                 abs(bullet->y + bullet->rad * sign_y + step_bullet * sin(bullet->angle) - (map->walls + i)->y1) <=
                  thick) && ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                 (abs(bullet->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                  abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + bullet->rad + thick * 2))
                bullet->angle = 2 * M_PI - bullet->angle;

                    else if ((bullet->y + bullet->rad * sign_y >=
                              (map->walls + i)->y1 + thick ||
                              bullet->y + bullet->rad * sign_y + step_bullet * sin(bullet->angle) >=
                               (map->walls + i)->y1 + thick) &&
                              (bullet->y + bullet->rad * sign_y <= (map->walls + i)->y1 - thick) &&
                              ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                              (abs(bullet->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                               abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + bullet->rad + thick * 2))
                bullet->angle = 2 * M_PI - bullet->angle;
        }
    }
    for (int j = 0; j < 8; j++) {
        short ur = -1, ul = -1, ru = -1, rd = -1, lu = -1, ld = -1, dr = -1, dl = -1;
        if (vert / numberofColumns != 0) {
            if (vert % numberofColumns != numberofColumns - 1) ur = left(vert - 7);
            if (vert % numberofColumns != 0) ul = right(vert - 9);
        }
        if (vert % numberofColumns != numberofColumns - 1) {
            ru = up(vert + 1);
            rd = down(vert + 1);
        }
        if (vert % numberofColumns != 0) {
            lu = up(vert - 1);
            ld = down(vert - 1);
        }
        if (vert / numberofColumns != numberofColumns - 1) {
            dr = right(vert + 8);
            dl = left(vert + 8);
        }
        int b[] = {ur, ul, ru, rd, lu, ld, dr, dl};
        int i = b[j];
        if (i != -1 && (map->walls + i)->boolian) {
            if ((map->walls + i)->x1 - (map->walls + i)->x2 == 0)
                if ((pow((map->walls + i)->x1 - (bullet->x), 2) +
                     pow((map->walls + i)->y1 - (bullet->y), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                    (pow((map->walls + i)->x2 - (bullet->x), 2) +
                     pow((map->walls + i)->y1 - (bullet->y), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                    (pow((map->walls + i)->x1 - (bullet->x + step_bullet * cos(bullet->angle)), 2) +
                     pow((map->walls + i)->y1 - (bullet->y + step_bullet * sin(bullet->angle)), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                    (pow((map->walls + i)->x2 - (bullet->x + step_bullet * cos(bullet->angle)), 2) +
                     pow((map->walls + i)->y1 - (bullet->y + step_bullet * sin(bullet->angle)), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)))
                    bullet->angle = 2 * M_PI - bullet->angle;

            if ((map->walls + i)->y1 - (map->walls + i)->y2 == 0)
                if ((pow((map->walls + i)->y1 - (bullet->y), 2) +
                     pow((map->walls + i)->x1 - (bullet->x), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                    (pow((map->walls + i)->y2 - (bullet->y), 2) +
                     pow((map->walls + i)->x1 - (bullet->x), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                    (pow((map->walls + i)->y1 - (bullet->y), 2) +
                     pow((map->walls + i)->x1 - (bullet->x), 2) <=
                     (bullet->rad + thick) * (bullet->rad + thick)) ||
                        (pow((map->walls + i)->y2 - (bullet->y), 2) +
                         pow((map->walls + i)->x1 - (bullet->x), 2) <=
                         (bullet->rad + thick) * (bullet->rad + thick)))
                    bullet->angle = M_PI - bullet->angle;
        }
    }
}

void bullet_collids_tanks(Bullet *bullet, Tank *tank, Shard *shard) {
    if (bullet->boolian) {
        for (int i = 0; i < numberofTanks; i++) {
            if ((tank + i)->boolian) {
                if (pow((tank + i)->x - bullet->x, 2) + pow((tank + i)->y - bullet->y, 2) <=
                    pow(radius_circle + bullet->rad, 2)) {
                    if (bullet->rad == radius_bullet) bullet->boolian = false;
                    (tank + i)->boolian = false;
                    remaining--;
                    if (bullet->rad == radius_fragBomb) {
                        for (int j = i * numberofShards; j < (i + 1) * numberofShards; j++) {
                            (shard + j)->x = bullet->x;
                            (shard + j)->y = bullet->y;
                            (shard + j)->n = 0;
                            (shard + j)->angle = (rand() % 360) * (M_PI / 180);
                            (shard + j)->boolian = true;
                        }
                        (tank + i)->fragBomb = 0;
                        bullet->rad = radius_bullet;
                    }
                    bullet->x = -100;
                    bullet->y = -100;
                    return;
                }
                if (pow((tank + i)->x + shooter * cos((tank + i)->angle) - bullet->x, 2) +
                    pow((tank + i)->y + shooter * sin((tank + i)->angle) - bullet->y, 2) <=
                    pow(radius_shooter + bullet->rad, 2)) {
                    if (bullet->rad == radius_bullet) bullet->boolian = false;
                    (tank + i)->boolian = false;
                    remaining--;
                    if (bullet->rad == radius_fragBomb) {
                        for (int j = i * numberofShards; j < (i + 1) * numberofShards; j++) {
                            (shard + j)->x = bullet->x;
                            (shard + j)->y = bullet->y;
                            (shard + j)->n = 0;
                            (shard + j)->angle = (rand() % 360) * (M_PI / 180);
                            (shard + j)->boolian = true;
                        }
                        bullet->x = -100;
                        bullet->y = -100;
                        (tank + i)->fragBomb = 0;
                        bullet->rad = radius_bullet;
                    }
                }
            }
        }
        //printf("remaining: %d\n", remaining);
    }
}

void tank_collids_Items(Item *item, Tank *tank) {
    if (!tank->item && pow(tank->x - item->x, 2) + pow(tank->y - item->y, 2) <= pow(radius_circle + radius_item, 2)) {
        tank->item = true;
        printf("i got item\n");
        if (item->type == FragBomb) tank->fragBomb = 1;
        if (item->type == MineItem) tank->mine = 1;
        if (item->type == Lazer) tank->lazer = 1;
        item->boolian = false;
        return;
    }
    if (!tank->item && pow(tank->x + shooter * cos(tank->angle) - item->x, 2) +
                       pow(tank->y + shooter * sin(tank->angle) - item->y, 2) <=
                       pow(radius_shooter + radius_item, 2)) {
        printf("i got item\n");
        tank->item = true;
        if (item->type == FragBomb) tank->fragBomb = 1;
        if (item->type == MineItem) tank->mine = 1;
        if (item->type == Lazer) tank->lazer = 1;
        item->boolian = false;
    }
}

void shard_collids_tanks(Shard *shard, Tank *tank) {
    if ((pow(shard->x - cos(shard->angle) - tank->x, 2) + pow(shard->y - sin(shard->angle) - tank->y, 2) <=
         pow(thick + radius_circle, 2)) ||
        (pow(shard->x + cos(shard->angle) - tank->x, 2) + pow(shard->y + sin(shard->angle) - tank->y, 2) <=
         pow(thick + radius_circle, 2)) ||
        (pow(shard->x - cos(shard->angle) - (tank->x + shooter * cos(tank->angle)), 2) +
         pow(shard->y - sin(shard->angle) - (tank->y + shooter * sin(tank->angle)), 2) <=
         pow(thick + radius_shooter, 2)) ||
        (pow(shard->x + cos(shard->angle) - (tank->x + shooter * cos(tank->angle)), 2) +
         pow(shard->y + sin(shard->angle) - (tank->y + shooter * sin(tank->angle)), 2) <=
         pow(thick + radius_shooter, 2))) {
        tank->boolian = false;
        shard->boolian = false;
        remaining--;
    }
}

bool shard_collids_walls(Shard *shard, Map *map) {
    short r = 4;
    short vert = vertex(shard->x + r * cos(shard->angle), shard->y + r * sin(shard->angle));
    short x = shard->x + r * cos(shard->angle);
    short y = shard->y + r * sin(shard->angle);
    for (int j = 0; j < 4; j++) {
        short l = left(vert);
        short r = right(vert);
        short u = up(vert);
        short d = down(vert);
        int b[] = {r, d, l, u};
        int i = b[j];
        if ((map->walls + i)->boolian) {
            if ((abs(x - (map->walls + i)->x1) <= thick) &&
                (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                 (abs(y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                  abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick)))
                return false;
            if ((abs(y - (map->walls + i)->y1) <= thick) &&
                (((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                 (abs(x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                  abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick)))
                return false;
        }
    }

    if (shard->x < house / 2 || shard->y < house / 2 || shard->x > MAP_WIDTH - house / 2 - house ||
        shard->y > MAP_HEIGHT - house / 2)
        return false;
    return true;
}

void tank_collids_mine(Mine *mine, Tank *tank1) {
    if (pow(tank1->x - mine->x, 2) + pow(tank1->y - mine->y, 2) <= pow(radius_circle + radius_mine, 2)) {
        mine->n = 200;
        return;
    }
    if (pow(tank1->x + shooter * cos(tank1->angle) - mine->x, 2) +
        pow(tank1->y + shooter * sin(tank1->angle) - mine->y, 2) <= pow(radius_shooter + radius_mine, 2)) {
        mine->n = 200;
    }
}

void lazer_collids_walls(Tank *tank1, Tank *tank2, int *x, int *y) {
    int sign_x = (cos(tank1->angle) > 0 ? 1 : -1);
    int sign_y = (sin(tank1->angle) > 0 ? 1 : -1);
    if (sign_x == 1 && sign_y == -1) {
        if (cos(tank1->angle) >= cos(M_PI / 4)) {
            *x = MAP_WIDTH - house / 2 - house;
            *y = tank1->y + (*x - tank1->x) * tan(tank1->angle);
        } else {
            *y = house / 2;
            *x = tank1->x + (*y - tank1->y) / tan(tank1->angle);
        }
    }
    if (sign_x == 1 && sign_y == 1) {
        if (cos(tank1->angle) >= cos(M_PI / 4)) {
            *x = MAP_WIDTH - house / 2 - house;
            *y = tank1->y + (*x - tank1->x) * tan(tank1->angle);
        } else {
            *y = MAP_HEIGHT - house / 2;
            *x = tank1->x + (*y - tank1->y) / tan(tank1->angle);
        }
    }
    if (sign_x == -1 && sign_y == 1) {
        if (cos(tank1->angle) >= cos(3 * M_PI / 4)) {
            *y = MAP_HEIGHT - house / 2;
            *x = tank1->x + (*y - tank1->y) / tan(tank1->angle);
        } else {
            *x = house / 2;
            *y = tank1->y + (*x - tank1->x) * tan(tank1->angle);
        }
    }
    if (sign_x == -1 && sign_y == -1) {
        if (cos(tank1->angle) >= cos(3 * M_PI / 4)) {
            *y = house / 2;
            *x = tank1->x + (*y - tank1->y) / tan(tank1->angle);
        } else {
            *x = house / 2;
            *y = tank1->y + (*x - tank1->x) * tan(tank1->angle);
        }
    }
    if (*x > MAP_WIDTH - house / 2 - house) *x = MAP_WIDTH - house / 2 - house;
    if (*y > MAP_HEIGHT - house / 2) *y = MAP_HEIGHT - house / 2;
    if (*x < house / 2) *x = house / 2;
    if (*y < house / 2) *y = house / 2;

    double m = (double) (*y - tank1->y) / (double) (*x - tank1->x);
    double b = (double) tank1->y - m * (double) tank1->x;
    if (fabs((double) tank2->y - m * (double) tank2->x - b) / sqrt(1 + m * m) <= radius_circle + thick * 2) {
        tank2->boolian = false;
        if (remaining > 1) remaining--;
        //printf("remaining: %d\n", remaining);
        //fflush(stdout);
    }
}

///////////////for reading from file. it works correctly
/*

bool movement_collids_walls(Tank *tank, Map *map) {
    for (int i = 0; i < numberofWalls; i++) {
        int max_x, max_y;
        int sign_x = (cos(tank->angle) > 0 ? 1 : -1);
        int sign_y = (sin(tank->angle) > 0 ? 1 : -1);
        if ((radius_shooter + shooter) * cos(tank->angle) * sign_x > radius_circle)         max_x = (radius_shooter + shooter) * cos(tank->angle);
        else max_x = radius_circle * sign_x;
        if ((radius_shooter + shooter) * sin(tank->angle) * sign_y > radius_circle)         max_y = (radius_shooter + shooter) * sin(tank->angle);
        else max_y = radius_circle * sign_y;

        ////////////////////
        if ((((abs(tank->x + max_x + step * cos(tank->angle) - (map->walls + i)->x1) <= thick) && (state[SDL_SCANCODE_UP])) ||
                ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <= thick) && (state[SDL_SCANCODE_DOWN])))  &&
                (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <= abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + radius_circle + thick / 2))
                )
            return false;
        if ((((abs(tank->y - max_y - step * sin(tank->angle) - (map->walls + i)->y1) <= thick) && (state[SDL_SCANCODE_UP])) ||
                ((abs(tank->y + radius_circle * sign_y + step * cos(tank->angle) - (map->walls + i)->y1) <= thick) && (state[SDL_SCANCODE_DOWN]))) &&
                ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <= abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + radius_circle + thick / 2))
            return false;

        if (!(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_LEFT])) {
            if (((((tank->y - radius_circle * sign_y - step * sin(tank->angle) >
                    min((map->walls + i)->y1, (map->walls + i)->y2)) &&
                   (tank->y - radius_circle * sign_y <= min((map->walls + i)->y1, (map->walls + i)->y2))) ||
                  ((tank->y - radius_circle * sign_y - step * sin(tank->angle) <
                    max((map->walls + i)->y1, (map->walls + i)->y2)) &&
                   (tank->y - radius_circle * sign_y >= max((map->walls + i)->y1, (map->walls + i)->y2))) &&
                  (state[SDL_SCANCODE_UP])) ||
                 ((abs(tank->y + radius_circle * sign_y + step * sin(tank->angle) - (map->walls + i)->y1) <= step) &&
                  (state[SDL_SCANCODE_DOWN]))) &&
                (abs(tank->x - (map->walls + i)->x1) <= step) && ((map->walls + i)->x1 - (map->walls + i)->x2 == 0))
                return false;
            if (((((tank->x + radius_circle * sign_x + step * cos(tank->angle) >
                    min((map->walls + i)->x1, (map->walls + i)->x2)) &&
                   (tank->x + radius_circle * sign_x <= min((map->walls + i)->x1, (map->walls + i)->x2))) ||
                  ((tank->x + radius_circle * sign_x + step * cos(tank->angle) <
                    max((map->walls + i)->x1, (map->walls + i)->x2)) &&
                   (tank->x + radius_circle * sign_x >= max((map->walls + i)->x1, (map->walls + i)->x2))) &&
                  (state[SDL_SCANCODE_UP])) ||
                 ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <= step) &&
                  (state[SDL_SCANCODE_DOWN]))) &&
                (abs(tank->y - (map->walls + i)->y1) <= step) && ((map->walls + i)->y1 - (map->walls + i)->y2 == 0))
                return false;
        }

        ////////////////////
        if ((((abs(tank->x + (shooter + radius_shooter) * cos(tank->angle - degree) - (map->walls + i)->x1) <= thick) && (state[SDL_SCANCODE_RIGHT])) ||
                ((abs(tank->x + (shooter + radius_shooter) * cos(tank->angle + degree) - (map->walls + i)->x1) <= thick) && (state[SDL_SCANCODE_LEFT]))) &&
                ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <= abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick))
            return false;
        if ((((abs(tank->y - (shooter + radius_shooter) * sin(tank->angle - degree) - (map->walls + i)->y1) <= thick) && (state[SDL_SCANCODE_RIGHT])) ||
                ((abs(tank->y - (shooter + radius_shooter) * sin(tank->angle + degree) - (map->walls + i)->y1) <= thick) && (state[SDL_SCANCODE_LEFT]))) &&
                ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <= abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick))
            return false;
    }
    return true;
}
*/
