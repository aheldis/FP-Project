#include "structs.h"
#ifndef PROJECT_LOGIC_H
#define PROJECT_LOGIC_H
bool movement_collids_walls(Tank *tank, Map *map, int flag);
bool turning_collids_walls(Tank *tank, Map *map);
void bullet_collids_walls(Bullet *bullet, Map *map);
#endif //PROJECT_LOGIC_H
