#include "structs.h"
#ifndef PROJECT_LOGIC_H
#define PROJECT_LOGIC_H
extern int remaining;
bool movement_collids_walls(Tank *tank, Map *map, int flag);
bool turning_collids_walls(Tank *tank, Map *map);
void bullet_collids_walls(Bullet *bullet, Map *map);
void bullet_collids_tanks(Bullet *bullet, Tank *tank);
#endif //PROJECT_LOGIC_H
