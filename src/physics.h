#include "structs.h"
#ifndef PROJECT_PHYSICS_H
#define PROJECT_PHYSICS_H
//int event(Tank* tank);
bool start_flag;
void move_tank(Tank *tank, Map *map);
void turn_tank(Tank *tank, Map *map);
void fire(Tank* tank, Shard *shard, Mine *mine);
void move_bullet(Bullet* bullet);
void move_shard(Shard *shard, Map *map);
#endif //PROJECT_PHYSICS_H
