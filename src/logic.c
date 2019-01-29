#include "structs.h"

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
    //////the house where tank is in it
    short vert = vertex(tank->x, tank->y);
    int max_x, max_y;
    int sign_x = (cos(tank->angle) > 0 ? 1 : -1);
    int sign_y = (sin(tank->angle) > 0 ? 1 : -1);
    if (((radius_shooter + step) * cos(tank->angle) + shooter * sign_x) * sign_x >
        radius_circle + step * cos(tank->angle))
        max_x = (radius_shooter + step) * cos(tank->angle) + shooter * sign_x;
    else max_x = radius_circle * sign_x + step * cos(tank->angle);
    if (((radius_shooter + step) * sin(tank->angle) + shooter * sign_y) * sign_y >
        radius_circle + step * sin(tank->angle))
        max_y = (radius_shooter + step) * sin(tank->angle) + shooter * sign_y;
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
                if ((pow((map->walls + i)->x1 - thick -
                         (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     pow((map->walls + i)->y1 - (tank->y + step * sin(tank->angle)), 2) <=
                     max_x * max_x * state[tank->up] + radius_circle * radius_circle * state[tank->down]) ||
                    (pow((map->walls + i)->x2 + thick -
                         (tank->x + step * cos(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     pow((map->walls + i)->y1 - (tank->y + step * sin(tank->angle)), 2) <=
                     max_x * max_x * state[tank->up] + radius_circle * radius_circle * state[tank->down])) {
                    if (flag == 0 && (((sign_x == 1 && i == r) || (sign_x == -1 && i == l)) && state[tank->up]) ||
                        (((sign_x == 1 && i == l) || (sign_x == -1 && i == r)) && state[tank->down]))
                        return false;
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
                if ((pow((map->walls + i)->y1 - thick -
                         (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     pow((map->walls + i)->x1 - (tank->x + step * cos(tank->angle)), 2) <=
                     max_y * max_y * state[tank->up] + radius_circle * radius_circle * state[tank->down]) ||
                    (pow((map->walls + i)->y2 + thick -
                         (tank->y + step * sin(tank->angle) * (state[tank->up] - state[tank->down])), 2) +
                     pow((map->walls + i)->x1 - (tank->x + step * cos(tank->angle)), 2) <=
                     max_y * max_y * state[tank->up] + radius_circle * radius_circle * state[tank->down])) {
                    if (flag == 1 && (((sign_y == 1 && i == d) || (sign_y == -1 && i == u)) && state[tank->up]) ||
                        (((sign_y == 1 && i == u) || (sign_y == -1 && i == d)) && state[tank->down]))
                        return false;
                }
        }
    }
    return true;
}

bool turning_collids_walls(Tank *tank, Map *map) {
    //////the house where tank is in it
    short vert = vertex(tank->x, tank->y);
    double deg = (tank->angle + degree) * state[tank->right] + (tank->angle - degree) * state[tank->left];
    int max_x, max_y;
    int sign_x = (cos(deg) > 0 ? 1 : -1);
    int sign_y = (sin(deg) > 0 ? 1 : -1);
    if (((radius_shooter + step) * cos(deg) + shooter * sign_x) * sign_x >
        radius_circle + step * cos(deg))
        max_x = (radius_shooter + step) * cos(deg) + shooter * sign_x;
    else max_x = radius_circle * sign_x + step * cos(deg);
    if (((radius_shooter + step) * sin(deg) + shooter * sign_y) * sign_y >
        radius_circle + step * sin(deg))
        max_y = (radius_shooter + step) * sin(deg) + shooter * sign_y;
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
                 abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + radius_circle + thick))
                return false;
            if ((((abs(
                    tank->y + radius_shooter * sign_y + shooter * sin(tank->angle + degree) - (map->walls + i)->y1) <=
                   thick) && (state[tank->right])) ||
                 ((abs(tank->y + radius_shooter * sign_y + shooter * sin(tank->angle - degree) -
                       (map->walls + i)->y1) <=
                   thick) && (state[tank->left]))) &&
                ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                 abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + radius_circle + thick))
                return false;
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
                            (map->walls + i)->x1) <=
                           thick + 1) && (state[tank->right])) ||
                         ((abs(tank->x + radius_shooter * sign_x + shooter * cos(tank->angle - degree) -
                               (map->walls + i)->x1) <=
                           thick + 1) && (state[tank->left]))) &&
                        ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                        (abs(tank->y + radius_shooter * sign_y + shooter * sin(deg) -
                             ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <
                         abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + shooter + radius_shooter + thick))
                        return false;
                    if ((((abs(
                            tank->y + radius_shooter * sign_y + shooter * sin(tank->angle + degree) -
                            (map->walls + i)->y1) <=
                           thick + 1) && (state[tank->right])) ||
                         ((abs(tank->y + radius_shooter * sign_y + shooter * sin(tank->angle - degree) -
                               (map->walls + i)->y1) <=
                           thick + 1) && (state[tank->left]))) &&
                        ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                        (abs(tank->x + radius_shooter * sign_x + shooter * cos(deg) -
                             ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <
                         abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + shooter + radius_shooter + thick))
                        return false;
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
            if ((abs(bullet->x + radius_bullet * sign_x + step_bullet * cos(bullet->angle) - (map->walls + i)->x1) <=
                 thick) && ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                (abs(bullet->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                 abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + radius_bullet + thick * 2))
                bullet->angle = M_PI - bullet->angle;
            else if ((bullet->x + radius_bullet * sign_x + step_bullet * cos(bullet->angle) >=
                      (map->walls + i)->x1 + thick) &&
                     (bullet->x + radius_bullet * sign_x <= (map->walls + i)->x1 - thick) &&
                     ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                     (abs(bullet->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                      abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + radius_bullet + thick * 2))
                bullet->angle = M_PI - bullet->angle;
            if ((abs(bullet->y + radius_bullet * sign_y + step_bullet * sin(bullet->angle) - (map->walls + i)->y1) <=
                 thick) && ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                (abs(bullet->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                 abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + radius_bullet + thick * 2))
                bullet->angle = 2 * M_PI - bullet->angle;
            else if ((bullet->y + radius_bullet * sign_y + step_bullet * sin(bullet->angle) >=
                      (map->walls + i)->y1 + thick) &&
                     (bullet->y + radius_bullet * sign_y <= (map->walls + i)->y1 - thick) &&
                     ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                     (abs(bullet->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                      abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + radius_bullet + thick * 2))
                bullet->angle = 2 * M_PI - bullet->angle;
        }
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
}*/
