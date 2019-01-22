#include "structs.h"

SDL_Window *window;
SDL_Renderer *renderer;
const Uint8 *state;
int red = 150, green = 150, blue = 150;

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
    static int r = 30, g = 30, b = 30;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    SDL_Event event;
    state = SDL_GetKeyboardState(NULL);
    while( SDL_PollEvent(&event) != 0 ) {
        ///a->97
//        printf ("%lld\n", event.key.keysym.sym);
        ///a->4
//        printf ("%lld\n", SDL_SCANCODE_A);
        if (event.type == SDL_QUIT) {
            free(map->tanks->bullets);
            free(map->tanks);
            free(map);
            quit_window();
        }
    }
    if (state[SDL_SCANCODE_N]) {
        SDL_Delay(300);
        r = red_white - r, g = green_white - g, b = blue_white - b;
        red = red_white - red, green = green_white - green, blue = blue_white - blue;
    }
}

void draw_tank(Tank *tank) {
    filledCircleRGBA(renderer, tank->x, tank->y, radius_circle, tank->r, tank->g, tank->b, 255);
    filledCircleRGBA(renderer, tank->x + shooter * cos(tank->angle), tank->y - shooter * sin(tank->angle), radius_shooter, 75, 75, 75, 255);
    if (red == 105) {
        circleRGBA(renderer, tank->x, tank->y, radius_circle, 75, 75, 75, 255);
        circleRGBA(renderer, tank->x, tank->y, radius_circle + 1, 75, 75, 75, 255);
    }
}

void draw_bullet(Bullet *bullet) {
    if (bullet->boolian){
        filledCircleRGBA(renderer, bullet->x, bullet->y, radius_bullet, 150, 150, 150, 255);
        move_bullet(bullet);
    }
}

void draw_walls(Wall* walls) {
    thickLineRGBA(renderer, walls->x1, walls->y1, walls->x2, walls->y2, thick, red, green, blue, 255);
}
