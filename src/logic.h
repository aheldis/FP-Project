#include "structs.h"
#ifndef PROJECT_LOGIC_H
#define PROJECT_LOGIC_H
extern int remaining;
bool movement_collids_walls(Tank *tank, Map *map, int flag);
bool turning_collids_walls(Tank *tank, Map *map);
void bullet_collids_walls(Bullet *bullet, Map *map);
void bullet_collids_tanks(Bullet *bullet, Tank *tank, Shard *shard);
void tank_collids_fragBombItem(FragBomb *fragBomb, Tank *tank);
void shard_collids_tanks(Shard *shard, Tank *tank);
bool shard_collids_walls(Shard *shard, Map *map);
#endif //PROJECT_LOGIC_H
