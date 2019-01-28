#include "structs.h"
#ifndef PROJECT_PHYSICS_H
#define PROJECT_PHYSICS_H
//int event(Tank* tank);
void move_tank(Tank *tank, Map *map);
void turn_tank(Tank *tank, Map *map);
void fire(Tank* tank);
void move_bullet(Bullet* bullet);
#endif //PROJECT_PHYSICS_H
