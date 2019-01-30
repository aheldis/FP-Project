#include "structs.h"

SDL_Window *window;
SDL_Renderer *renderer;
const Uint8 *state;
int rback = 30, gback = 30, bback = 30;

void init_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Alter Tank", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_WIDTH, MAP_HEIGHT,
                              SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void quit_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Keycode handle_events(Map *map) {
    SDL_Keycode keycode = 0;
    SDL_SetRenderDrawColor(renderer, rback, gback, bback, a);
    SDL_RenderClear(renderer);
    SDL_Event event;
    state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event) != 0) {
        if (event.key.keysym.sym) keycode = event.key.keysym.sym;
        if (event.type == SDL_QUIT) {
            free(map->tanks->bullets);
            free(map->tanks);
            free(map);
            quit_window();
        }
    }
    if ((state[224] || state[228]) && state[SDL_SCANCODE_N]) {
        SDL_Delay(300);
        rback = red_white - rback, gback = green_white - gback, bback = blue_white - bback;
    }
    return keycode;
}

void draw_tank(Tank *tank) {
    int red, green, blue;
    if (rback == 225) red = 128, green = 128, blue = 128;
    else red = 75, green = 75, blue = 75;
    filledCircleRGBA(renderer, tank->x, tank->y, radius_circle, tank->r, tank->g, tank->b, a);
    filledCircleRGBA(renderer, tank->x + shooter * cos(tank->angle), tank->y + shooter * sin(tank->angle),
                     radius_shooter, red, green, blue, a);
    if (rback == 225) {
        circleRGBA(renderer, tank->x, tank->y, radius_circle, red, green, blue, a);
        circleRGBA(renderer, tank->x, tank->y, radius_circle - 1, red, green, blue, a);
    }
}

void draw_bullet(Bullet *bullet) {
    if (bullet->boolian && bullet->x != -100)
        filledCircleRGBA(renderer, bullet->x, bullet->y, bullet->rad, 150, 150, 150, a);
}

void draw_shard(Shard *shard) {
    int r = 4;
    thickLineRGBA(renderer, shard->x - r * cos(shard->angle), shard->y - r * sin(shard->angle),
                  shard->x + r * cos(shard->angle), shard->y + r * sin(shard->angle), thick, 150, 150, 150, a);
}

void draw_fragBomb(FragBomb *fragBomb) {
    int red = 80, green = 80, blue = 80;
//    if (rback == 225) red = red_white - red, green = green_white - green, blue = blue_white - blue;
    filledCircleRGBA(renderer, fragBomb->x, fragBomb->y, radius_item, red_white, green_black, blue_black, a);
    circleRGBA(renderer, fragBomb->x, fragBomb->y, radius_item, red, green, blue, a);
    circleRGBA(renderer, fragBomb->x, fragBomb->y, radius_item - 1, red, green, blue, a);
    SDL_RenderSetScale(renderer, 1.3, 1.3);
    stringRGBA(renderer, (fragBomb->x - 4) / 1.3, (fragBomb->y - 3) / 1.3, "*", 50, 50, 50, a);
    SDL_RenderSetScale(renderer, 1, 1);
}

void draw_walls(Wall *walls) {
    int red = 150, green = 150, blue = 150;
    if (rback == 225) red = red_white - red, green = green_white - green, blue = blue_white - blue;
    thickLineRGBA(renderer, walls->x1, walls->y1, walls->x2, walls->y2, thick, red, green, blue, a);
}
