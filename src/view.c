#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>

#include "structs.h"
#include "physics.h"

#define red 255
#define green 255
#define blue 255
#define a 255
#define MAP_WIDTH 700
#define MAP_HEIGHT 700
#define radius_circle 15
#define shooter 20
#define radius_shooter 5
#define radius_bullet 3

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Keycode keycode;

void init_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Alter Tank", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_WIDTH, MAP_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void quit_window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handle_events(Map *map){
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, red, green, blue, a);
    SDL_RenderClear(renderer);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit_window();
                break;
            case SDL_KEYDOWN:
                keycode = event.key.keysym.sym;
                break;
            default:
                keycode = 0;
                break;
        }
        fire(map->tanks);
        move_tank(map->tanks);
        turn_tank(map->tanks);
    }
}

void draw_tank(Tank *tank) {
    filledCircleRGBA(renderer, tank->x, tank->y, radius_circle, tank->r, tank->g, tank->b, 255);
    filledCircleRGBA(renderer, tank->x + shooter * cos(180 / M_PI * tank->angle), tank->y - shooter * sin(180 / M_PI * tank->angle), radius_shooter, tank->r, tank->g, tank->b, 255);
}

void draw_bullet(Bullet *bullet) {
    if (bullet->boolian){
        filledCircleRGBA(renderer, bullet->x, bullet->y, radius_bullet, 0, 0, 0, 255);
        move_bullet(bullet);
    }
}

void draw_walls(Wall* walls) {
    lineRGBA(renderer, walls->x1, walls->y1, walls->x2, walls->y2, 0, 0, 0, 255);
}
