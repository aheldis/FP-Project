#include "structs.h"

SDL_Window *window;
SDL_Renderer *renderer;
const Uint8 *state;

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
    SDL_SetRenderDrawColor(renderer, red - 200, green - 200, blue - 200, a);
    SDL_RenderClear(renderer);
    SDL_Event event;
    state = SDL_GetKeyboardState(NULL);
    while( SDL_PollEvent(&event) != 0 ) {
        if (event.type == SDL_QUIT) {
            free(map->tanks->bullets);
            free(map->tanks);
            free(map);
            quit_window();
        }
    }
    fire(map->tanks);
    if (movement_collids_walls(map->tanks, map)) {
        move_tank(map->tanks);
        turn_tank(map->tanks);
    }
}

void draw_tank(Tank *tank) {
    filledCircleRGBA(renderer, tank->x, tank->y, radius_circle, tank->r, tank->g, tank->b, 255);
    filledCircleRGBA(renderer, tank->x + shooter * cos(tank->angle), tank->y - shooter * sin(tank->angle), radius_shooter, 100, 100, 100, 255);
}

void draw_bullet(Bullet *bullet) {
    if (bullet->boolian){
        filledCircleRGBA(renderer, bullet->x, bullet->y, radius_bullet, 150, 150, 150, 255);
        move_bullet(bullet);
    }
}

void draw_walls(Wall* walls) {
    thickLineRGBA(renderer, walls->x1, walls->y1, walls->x2, walls->y2, thick, 200, 200, 200, 255);
}
