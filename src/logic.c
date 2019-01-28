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
    if ((radius_shooter + shooter + step) * cos(tank->angle) * sign_x > radius_circle + step * cos(tank->angle))
        max_x = (radius_shooter + shooter + step) * cos(tank->angle);
    else max_x = (radius_circle + step * cos(tank->angle)) * sign_x;
    if ((radius_shooter + shooter + step) * sin(tank->angle) * sign_y > radius_circle + step * sin(tank->angle))
        max_y = (radius_shooter + shooter + step) * sin(tank->angle);
    else max_y = (radius_circle + step * sin(tank->angle)) * sign_y;

    int max_t = max(max_x, max_y);

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
                if ((((abs(tank->x + max_x - (map->walls + i)->x1) <= thick * 2) && (state[tank->up])) ||
                     ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <=
                       thick * 2) &&
                      (state[tank->down]))) &&
                    (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                     (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                      abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick * 2)))
                    return false;
            if (flag == 1)
                if ((((abs(tank->y + max_y - (map->walls + i)->y1) <= thick * 2) && (state[tank->up])) ||
                     ((abs(tank->y - radius_circle * sign_y - step * cos(tank->angle) - (map->walls + i)->y1) <=
                       thick * 2) &&
                      (state[tank->down]))) &&
                    ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                    (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                     abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2  + thick * 2))
                    return false;
        }
    }
    short vertl = vertex(tank->x - max_x * sign_x, tank->y);
    short vertr = vertex(tank->x + max_x * sign_x, tank->y);
    short vertu = vertex(tank->x, tank->y + max_y * sign_y);
    short vertd = vertex(tank->x, tank->y - max_y * sign_y);
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
                        if ((((abs(tank->x + max_x - (map->walls + i)->x1) <= thick * 2) && (state[tank->up])) ||
                             ((abs(tank->x - radius_circle * sign_x - step * cos(tank->angle) - (map->walls + i)->x1) <=
                               thick * 2) &&
                              (state[tank->down]))) &&
                            (((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                             (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                              abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick * 2)))
                            return false;
                    if (flag == 1)
                        if ((((abs(tank->y + max_y - (map->walls + i)->y1) <= thick * 2) && (state[tank->up])) ||
                             ((abs(tank->y - radius_circle * sign_y - step * cos(tank->angle) - (map->walls + i)->y1) <=
                               thick * 2) &&
                              (state[tank->down]))) &&
                            ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                            (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                             abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick * 2))
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
                    //printf("%d %d %d\n", vert, vertu, vertd);

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
                //if (i && (map->walls + i)->boolian && !(map->walls + left(vertr))->boolian)

            }
        }
        return true;
    }

    bool turning_collids_walls(Tank *tank, Map *map) {
        short vert = vertex(tank->x, tank->y);
        int max_x, max_y;
        int sign_x = (cos(tank->angle) > 0 ? 1 : -1);
        int sign_y = (sin(tank->angle) > 0 ? 1 : -1);
        if ((radius_shooter + shooter + step) * cos(tank->angle) * sign_x > radius_circle)
            max_x = (radius_shooter + shooter + step) * cos(tank->angle);
        else max_x = (radius_circle + step) * sign_x;
        if ((radius_shooter + shooter + step) * sin(tank->angle) * sign_y > radius_circle)
            max_y = (radius_shooter + shooter + step) * sin(tank->angle);
        else max_y = (radius_circle + step) * sign_y;
        short l1 = left(vert);
        short r1 = right(vert);
        short u1 = up(vert);
        short d1 = down(vert);
        short vertl = vertex(tank->x - max_x, tank->y);
        short vertr = vertex(tank->x + max_x, tank->y);
        short vertu = vertex(tank->x, tank->y + max_y);
        short vertd = vertex(tank->x, tank->y - max_y);
        short l2 = -1, r2 = -1, u2 = -1, d2 = -1;
        if (vert % numberofColumns != 0) l2 = down(vertu - 1);
        if (vert % numberofColumns != numberofColumns - 1) r2 = down(vertu + 1);
        if (vert / numberofColumns != 0) u2 = left(vertr - numberofColumns);
        if (vert / numberofColumns != numberofColumns - 1) d2 = left(vertr + numberofColumns);
        for (int j = 0; j < 8; j++) {
            int b[] = {r1, d1, l1, u1, r2, d2, l2, u2};
            int i = b[j];
            ////////////////////rotation
            if ((map->walls + i)->boolian && i != -1) {
                if ((((abs(tank->x + (shooter + radius_shooter) * cos(tank->angle + degree) - (map->walls + i)->x1) <=
                       thick) && (state[tank->right])) ||
                     ((abs(tank->x + (shooter + radius_shooter) * cos(tank->angle - degree) - (map->walls + i)->x1) <=
                       thick) && (state[tank->left]))) &&
                    ((map->walls + i)->x1 - (map->walls + i)->x2 == 0) &&
                    (abs(tank->y - ((map->walls + i)->y1 + (map->walls + i)->y2) / 2) <=
                     abs((map->walls + i)->y1 - (map->walls + i)->y2) / 2 + thick))
                    return false;
                if ((((abs(tank->y + (shooter + radius_shooter) * sin(tank->angle + degree) - (map->walls + i)->y1) <=
                       thick) && (state[tank->right])) ||
                     ((abs(tank->y + (shooter + radius_shooter) * sin(tank->angle - degree) - (map->walls + i)->y1) <=
                       thick) && (state[tank->left]))) &&
                    ((map->walls + i)->y1 - (map->walls + i)->y2 == 0) &&
                    (abs(tank->x - ((map->walls + i)->x1 + (map->walls + i)->x2) / 2) <=
                     abs((map->walls + i)->x1 - (map->walls + i)->x2) / 2 + thick))
                    return false;
            }
        }

        return true;
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
