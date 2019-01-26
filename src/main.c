#include "structs.h"

#ifdef main
#undef main
#endif

//////////////for random map

struct node {
    short int vertex;
    struct node *next;
};

struct node *createNode(int v) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph {
    short int numVertices;
    short int *visited;
    struct node **adjLists; // we need int** to store a two dimensional array. Similary, we need struct node** to store an array of Linked lists
};

struct Graph *createGraph(int vertices) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(struct node *));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
    // Add edge from src to dest
    struct node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Add edge from dest to src
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void DFS(struct Graph *graph, int vertex, Wall *walls) {
    struct node *adjList = graph->adjLists[vertex];
    struct node *temp = adjList;

    graph->visited[vertex] = 1;
    //printf("Visited %d \n", vertex);

    while (temp != NULL) {
        short int connectedVertex = temp->vertex;

        if (graph->visited[connectedVertex] == 0) {
            int i = max(connectedVertex, vertex);
            if (abs(connectedVertex - vertex) == 1)
                (walls + i % numberofColumns + i / numberofColumns * (numberofColumns + 1))->boolian = false;
            else (walls + i + numberofRows * (numberofColumns + 1))->boolian = false;
            DFS(graph, connectedVertex, walls);
        }
        temp = temp->next;
    }
}

void read_map_array(Map *map) {
    short int n = 0;
    /////////amoodi
    for (int i = 0; i < numberofRows; i++) {
        for (int j = 0; j <= numberofColumns; j++) {
            int rend = rand() % 4;
            (map->walls + n)->x1 = j * house + house / 2;
            (map->walls + n)->y1 = i * house + house / 2;
            (map->walls + n)->x2 = j * house + house / 2;
            (map->walls + n)->y2 = (i + 1) * house + house / 2;
            if (rend == 1 && j != 0 && j != numberofColumns) (map->walls + n)->boolian = false;
            else (map->walls + n)->boolian = true;
            n++;
        }
    }
    /////////ofoghi
    for (int i = 0; i <= numberofRows; i++) {
        for (int j = 0; j < numberofColumns; j++) {
            int rend = rand() % 4;
            (map->walls + n)->x1 = j * house + house / 2;
            (map->walls + n)->y1 = i * house + house / 2;
            (map->walls + n)->x2 = (j + 1) * house + house / 2;
            (map->walls + n)->y2 = i * house + house / 2;
            if (rend == 1 && i != 0 && i != numberofRows) (map->walls + n)->boolian = false;
            else (map->walls + n)->boolian = true;
            n++;
        }
    }
    struct Graph *graph = createGraph(numberofRows * numberofColumns);
    for (int j = 0; j < numberofRows; j++) {
        for (int i = j * numberofColumns; i < (j + 1) * numberofColumns; i++) {
            if (i != (j + 1) * numberofColumns - 1) addEdge(graph, i, i + 1);
            if (j != numberofRows - 1) addEdge(graph, i, i + numberofColumns);
        }
    }

    int rend = rand() % 64;
    DFS(graph, rend, map->walls);

}


//////////////for reading from file. it works correctly

void read_map_file(Map *map, char *file_path) {
    FILE *file1 = fopen(file_path, "r");
    int n, x1, y1, x2, y2;
    fscanf(file1, "%d\n", &n);
    for (int i = 0; i < n; i++) {
        fscanf(file1, "%d %d %d %d\n", &x1, &y1, &x2, &y2);
        (map->walls + i)->x1 = x1 * house;
        (map->walls + i)->y1 = y1 * house;
        (map->walls + i)->x2 = x2 * house;
        (map->walls + i)->y2 = y2 * house;
    }
    fclose(file1);
}


bool newGame(Tank *tank, Bullet *bullet, Map *map, Wall *walls, bool flag) {
    for (int i = 0; i < 2; i++) {
        (tank + i)->r = rand() % 255;
        (tank + i)->g = rand() % 255;
        (tank + i)->b = rand() % 255;
        (tank + i)->angle = 0;
        (tank + i)->left = 0;
        (tank + i)->up = 0;
        (tank + i)->down = 0;
        (tank + i)->right = 0;
        (tank + i)->shoot = 0;
    }

    SDL_Keycode keycode = 0;
    enum {
        tank1, tank2
    } j = 0;
    int i = 0, enter = 1;
    while (flag) {
        keycode = handle_events(map);
        static SDL_Keycode keycodepre = 0;
        int r = red_white - 20, g = green_white - 20, b = blue_white - 20;
        if (rback == 225) r = red_white - r, g = green_white - g, b = blue_white - b;

        int red[] = {r, r};
        int green[] = {g, g};
        int blue[] = {b, b};

        if (state[SDL_SCANCODE_ESCAPE]) {
            SDL_Delay(200);
            flag = false;
            return flag;
        }

        red[j] = 100;
        green[j] = 100;
        blue[j] = 100;

        stringRGBA(renderer, 150, 135, "The key that has been pressed:", r, g, b, a);
        if (keycodepre) stringRGBA(renderer, 150, 150, SDL_GetKeyName(keycodepre), r, g, b, a);

        int x = 200, y = 325;
        stringRGBA(renderer, x, y, "tank1", red[tank1], green[tank1], blue[tank1], a);
        y += 275;
        stringRGBA(renderer, x, y, "tank2", red[tank2], green[tank2], blue[tank2], a);

        tank->y = 250;
        tank->x = 216;
        draw_tank(tank);
        (tank + 1)->y = 525;
        (tank + 1)->x = 216;
        draw_tank(tank + 1);

        x = 516, y = 305;
        short vx[] = {x, x + 40, x + 40, x};
        short vy[] = {y, y, y + 40, y + 40};
        for (int k = 0; k < 2; k++) {
            int redj[] = {red[tank1 + k], (tank + k)->r};
            int greenj[] = {green[tank1 + k], (tank + k)->g};
            int bluej[] = {blue[tank1 + k], (tank + k)->b};
            int x1;
            int yes = i % 5 == 0 && k == i / 5;
            filledPolygonRGBA(renderer, vx, vy, 4, redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                if (enter) f[k] = 0;
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode) {
                        (tank + k)->left = SDL_GetScancodeFromKey(keycode);
                        f[k] = 1;
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->left && f[k]) i++;
                }
            }
            if ((tank + k)->left) {
                if (SDL_GetKeyFromScancode((tank + k)->left) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->left)), rback, gback, bback, a);
            }

            for (int z = 0; z < 4; z++) vx[z] += 45;
            x += 45;
            yes = i % 5 == 1 && k == i / 5;
            filledPolygonRGBA(renderer, vx, vy, 4, redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                if (enter) f[k] = 0;
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode) {
                        (tank + k)->down = SDL_GetScancodeFromKey(keycode);
                        f[k] = 1;
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->down && f[k]) i++;
                }
            }
            if ((tank + k)->down){
                if (SDL_GetKeyFromScancode((tank + k)->down) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->down)), rback, gback, bback, a);
            }

            for (int z = 0; z < 4; z++) vy[z] -= 45;
            y -= 45;

            yes = i % 5 == 2 && k == i / 5;
            filledPolygonRGBA(renderer, vx, vy, 4, redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                if (enter) f[k] = 0;
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode) {
                        (tank + k)->up = SDL_GetScancodeFromKey(keycode);
                        f[k] = 1;
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->up && f[k]) i++;
                }
            }
            if ((tank + k)->up){
                if (SDL_GetKeyFromScancode((tank + k)->up) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->up)), rback, gback, bback, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] += 45;
                vy[z] += 45;
            }
            x += 45;
            y += 45;

            yes = i % 5 == 3 && k == i / 5;
            filledPolygonRGBA(renderer, vx, vy, 4, redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                if (enter) f[k] = 0;
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode) {
                        (tank + k)->right = SDL_GetScancodeFromKey(keycode);
                        f[k] = 1;
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->right && f[k]) i++;
                }
            }
            if ((tank + k)->right){
                if (SDL_GetKeyFromScancode((tank + k)->right) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->right)), rback, gback, bback, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] -= 250;
                vy[z] -= 45;
            }
            x -= 250;
            y -= 45;

            yes = i % 5 == 4 && k == i / 5;
            filledPolygonRGBA(renderer, vx, vy, 4, redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                if (enter) f[k] = 0;
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode) {
                        (tank + k)->shoot = SDL_GetScancodeFromKey(keycode);
                        f[k] = 1;
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->shoot && f[k]) i++;
                }
            }
            if ((tank + k)->shoot){
                if (SDL_GetKeyFromScancode((tank + k)->shoot) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->shoot)), rback, gback, bback, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] += - 90 + 250;
                vy[z] += 45 + 275;
            }
            x += 250 - 90;
            y += 45 + 275;
            if (i >= 5) j = tank2;
        }
        if (i == 10)    break;
        enter = 0;
        if (keycodepre != keycode && keycode) keycodepre = keycode;
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    for (int i = 0; i < 2; i++) {
        (tank + i)->x = rand() % numberofRows * house + house;
        (tank + i)->y = rand() % numberofColumns * house + house;
        (tank + i)->angle = rand() % 360;
        (tank + i)->boolian = true;
        (tank + i)->score = 0;
    }

    while (tank->x == (tank + 1)->x && tank->y == (tank + 1)->y)
        (tank + 1)->y = rand() % numberofColumns * house + house;

    for (int i = 0; i < numberofBullets; i++) {
        (bullet + i)->boolian = true;
        (bullet + i)->x = -100;
        (bullet + i)->y = -100;
        (bullet + numberofBullets + i)->boolian = true;
        (bullet + numberofBullets + i)->x = -100;
        (bullet + numberofBullets + i)->y = -100;
    }
    tank->bullets = bullet;
    (tank + 1)->bullets = bullet + numberofBullets;
    map->tanks = tank;
    map->walls = walls;

    ///////////////for reading from file. it works correctly
    //read_map_file(map, "D:\\programming\\c\\University\\project\\project\\src\\map.txt");

    read_map_array(map);
    return flag;
}


bool menu(Tank *tank, Bullet *bullet, Map *map, Wall *walls, bool flag) {
    bool f = flag;
    Tank *sample1 = malloc(sizeof(Tank));
    sample1->x = MAP_WIDTH / 2;
    sample1->y = 750;
    sample1->angle = 0;
    sample1->r = 180;
    sample1->g = 180;
    sample1->b = 180;
    Tank *sample2 = malloc(sizeof(Tank));
    sample2->x = MAP_WIDTH / 2;
    sample2->y = MAP_HEIGHT - sample1->y;
    sample2->angle = M_PI;
    sample2->r = 180;
    sample2->g = 180;
    sample2->b = 180;
    enum {
        new, load, end, game
    } j = 0;
    while (flag) {
        handle_events(map);
        int r = red_white - 20, g = green_white - 20, b = blue_white - 20;
        if (rback == 225) r = red_white - r, g = green_white - g, b = blue_white - b;

        int red[] = {r, r, r};
        int green[] = {g, g, g};
        int blue[] = {b, b, b};


        if (state[SDL_SCANCODE_DOWN]) {
            static int n = 0;
            n++;
            if (n >= 5) {
                j = (j + 1) % 3;
                n = 0;
            }
        }
        if (state[SDL_SCANCODE_UP]) {
            static int n = 0;
            n++;
            if (n >= 5) {
                if (j > 0) j--;
                else j = 2;
                n = 0;
            }
        }

        if (state[SDL_SCANCODE_RETURN]) {
            if (j == new || j == load) flag = !newGame(tank, bullet, map, walls, flag);
            if (j == end) {
                flag = false;
                quit_window();
            }
        }

        if (state[SDL_SCANCODE_ESCAPE]) {
            static int n = 0;
            n++;
            if (n >= 5) flag = false;
        }

        int x = MAP_WIDTH / 2 - house / 2;
        int y = 300;

        red[j] = 100;
        green[j] = 100;
        blue[j] = 100;

        if (flag) {
            draw_tank(sample1);
            draw_tank(sample2);
            stringRGBA(renderer, x, y, "Start Game", red[new], green[new], blue[new], a);
            x += 4;
            y += 25;
            stringRGBA(renderer, x, y, "Load Game", red[load], green[load], blue[load], a);
            x += 3;
            y += 25;
            stringRGBA(renderer, x, y, "End Game", red[end], green[end], blue[end], a);
        }
        sample1->x += step * cos(sample1->angle);
        sample1->y -= step * sin(sample1->angle);
        sample1->angle += 0.1;
        if (sample1->x < 0) sample1->x = MAP_WIDTH - radius_circle;
        if (sample1->y < 0) sample1->y = MAP_HEIGHT - radius_circle;
        if (sample1->x > MAP_WIDTH) sample1->x = radius_circle;
        if (sample1->y > MAP_HEIGHT) sample1->y = radius_circle;
        sample2->angle += 0.1;
        sample2->x = MAP_WIDTH - sample1->x;
        sample2->y = MAP_HEIGHT - sample1->y;
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }
    if (f != flag) SDL_Delay(200);
    return flag;
}


int main(int argc, char *argv[]) {

    srand(time(0));
    Bullet *bullet = malloc(2 * sizeof(Bullet) * numberofBullets);
    Tank *tank = malloc(sizeof(Tank) * 2);
    Tank *sample = malloc(sizeof(Tank) * 2);
    Map *map = malloc(sizeof(Map) * 1);
    Wall *walls = malloc(sizeof(Wall) * numberofWalls);

    //////////////////menu
    init_window();
    bool flag = true;

    int x = MAP_WIDTH - 75, y = 290;
    for (int k = 0; k < 2; k++) {
        (sample + k)->angle = M_PI / 2;
        (sample + k)->x = x;
        (sample + k)->y = y;
        y += 240;
    }
    while (1) {
        int red = 150, green = 150, blue = 150;
        if (rback == 225) red = red_white - red, green = green_white - green, blue = blue_white - blue;
        flag = menu(tank, bullet, map, walls, flag);
        handle_events(map);
        for (int i = 0; i < numberofWalls; i++)
            if ((map->walls + i)->boolian)
                draw_walls(map->walls + i);
        for (int k = 0; k < 2; k++) {
            (sample + k)->r = (map->tanks + k)->r;
            (sample + k)->g = (map->tanks + k)->g;
            (sample + k)->b = (map->tanks + k)->b;
            for (int i = 0; i < numberofBullets; i++) draw_bullet((map->tanks + k)->bullets + i);
            draw_tank(map->tanks + k);
            draw_tank(sample + k);
            stringRGBA(renderer, x - 20, 310, "tank1", red, green, blue, a);
            stringRGBA(renderer, x - 20, 550, "tank2", red, green, blue, a);
            if (movement_collids_walls(map->tanks + k, map)) {
                move_tank(map->tanks + k);
                turn_tank(map->tanks + k);
            }
        }
        fire(map->tanks);
        if (state[SDL_SCANCODE_ESCAPE]) {
            flag = true;
            SDL_Delay(200);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FPS);
    }
}