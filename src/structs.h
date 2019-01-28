#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <time.h>

#define red_white 255
#define green_white 255
#define blue_white 255
#define red_black 0
#define green_black 0
#define blue_black 0
#define a 255
#define FPS 400
#define MAP_WIDTH 900
#define MAP_HEIGHT 820
#define house 90
#define numberofBullets 5
#define numberofWalls 144
#define numberofMaps 3
#define shooter 20
#define radius 15
#define radius_circle 15
#define radius_shooter 5
#define thick 2
#define step 4
#define step_bullet 6
#define degree 0.1
#define radius_bullet 3
#define distanceofBullets 500
#define numberofRows 8
#define numberofColumns 8
#define numberofTanks 2


#define max(a, b) (a >= b) * a + (a < b) * b
#define min(a, b) (a >= b) * b + (a < b) * a

typedef struct {
    int x, y;
    double angle;
    int n;
    bool boolian;
} Bullet;

typedef struct {
    int x;
    int y;
    int r;
    int g;
    int b;
    SDL_Scancode score;
    SDL_Scancode right;
    SDL_Scancode left;
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode shoot;
    char name[10];
    bool boolian;
    double angle;
    Bullet *bullets;
} Tank;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    bool boolian;
} Wall;

typedef struct {
    Tank *tanks;
    Wall *walls;
} Map;

#include "view.h"
#include "physics.h"
#include "logic.h"
#endif //PROJECT_STRUCTS_H
