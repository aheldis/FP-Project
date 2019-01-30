#include "structs.h"
#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const Uint8 *state;
extern int rback, gback, bback;
void init_window();
void quit_window();
void draw_tank(Tank *tank);
void draw_walls(Wall* walls);
void draw_bullet(Bullet* bullet);
void draw_shard(Shard *shard);
void draw_fragBomb(FragBomb *fragBomb);
SDL_Keycode handle_events(Map* map);
#endif //PROJECT_VIEW_H
