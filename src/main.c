#include "structs.h"

#ifdef main
#undef main
#endif

bool whilePlayingMenu = false, saveGame_flag = false;
int winner = 0;

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
};

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
    numberofRows = rand() % 3 + 5;
    numberofColumns = rand() % 7 + 5;
    MAP_WIDTH = (numberofColumns + 2) * house;
    MAP_HEIGHT = (numberofRows + 1) * house;
    numberofWalls = (numberofRows + 1) * numberofColumns + (numberofColumns + 1) * numberofRows;
    //free(map->walls);
    map->walls = malloc(sizeof(Wall) * numberofWalls);
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

    int rend = rand() % numberofRows * numberofColumns;
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


void nextGame(Tank *tank, Bullet *bullet, Map *map, Wall *walls) {
    ///////////////for reading from file. it works correctly
    //read_map_file(map, "D:\\programming\\c\\University\\project\\project\\src\\map.txt");

    read_map_array(map);

    for (int i = 0; i < 2; i++) {
        (tank + i)->x = rand() % numberofColumns * house + house;
        (tank + i)->y = rand() % numberofRows * house + house;
        (tank + i)->angle = (rand() % 360) * M_PI / 180;
        (tank + i)->boolian = true;
        (tank + i)->fragBomb = 0;
        (tank + i)->mine = 0;
        (tank + i)->lazer = 0;
        (tank + i)->item = false;
    }

    while (tank->x == (tank + 1)->x && tank->y == (tank + 1)->y)
        (tank + 1)->y = rand() % numberofColumns * house + house;

    for (int i = 0; i < numberofBullets; i++) {
        (bullet + i)->boolian = true;
        (bullet + i)->x = -100;
        (bullet + i)->y = -100;
        (bullet + i)->n = 0;
        (bullet + i)->rad = radius_bullet;
        (bullet + numberofBullets + i)->boolian = true;
        (bullet + numberofBullets + i)->x = -100;
        (bullet + numberofBullets + i)->y = -100;
        (bullet + numberofBullets + i)->n = 0;
        (bullet + numberofBullets + i)->rad = radius_bullet;
    }
    remaining = numberofTanks;
    start_flag = true;
}


bool newGame(Tank *tank, Bullet *bullet, Map *map, Wall *walls, bool flag) {
    for (int i = 0; i < numberofTanks; i++) {
        (tank + i)->r = rand() % 255;
        (tank + i)->g = rand() % 255;
        (tank + i)->b = rand() % 255;
        (tank + i)->angle = 0;
        (tank + i)->left = 0;
        (tank + i)->up = 0;
        (tank + i)->down = 0;
        (tank + i)->right = 0;
        (tank + i)->shoot = 0;
        for (int z = 0; z < 10; z++)
            (tank + i)->name[z] = 0;
    }

    SDL_Keycode keycode = 0;
    enum {
        tank1, tank2
    } j = 0;
    int i = 0, enter = 1;
    while (flag) {
        MAP_HEIGHT = 800, MAP_WIDTH = 800;
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

        int x = 150, y = 325;
        stringRGBA(renderer, x, y, "tank1", red[tank1], green[tank1], blue[tank1], a);
        y += 275;
        stringRGBA(renderer, x, y, "tank2", red[tank2], green[tank2], blue[tank2], a);

        tank->y = 275;
        tank->x = 166;
        draw_tank(tank);
        (tank + 1)->y = 550;
        (tank + 1)->x = 166;
        draw_tank(tank + 1);

        x = 466, y = 305;
        short vx[] = {x, x + 40, x + 40, x};
        short vy[] = {y, y, y + 40, y + 40};
        for (int k = 0; k < numberofTanks; k++) {
            int redj[] = {red[tank1 + k], (tank + k)->r};
            int greenj[] = {green[tank1 + k], (tank + k)->g};
            int bluej[] = {blue[tank1 + k], (tank + k)->b};
            int x1;
            int yes = i % 6 == 0 && k == i / 6;
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
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->left)), rback, gback,
                           bback, a);
            }

            for (int z = 0; z < 4; z++) vx[z] += 45;
            x += 45;
            yes = i % 6 == 1 && k == i / 6;
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
            if ((tank + k)->down) {
                if (SDL_GetKeyFromScancode((tank + k)->down) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->down)), rback, gback,
                           bback, a);
            }

            for (int z = 0; z < 4; z++) vy[z] -= 45;
            y -= 45;

            yes = i % 6 == 2 && k == i / 6;
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
            if ((tank + k)->up) {
                if (SDL_GetKeyFromScancode((tank + k)->up) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->up)), rback, gback,
                           bback, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] += 45;
                vy[z] += 45;
            }
            x += 45;
            y += 45;

            yes = i % 6 == 3 && k == i / 6;
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
            if ((tank + k)->right) {
                if (SDL_GetKeyFromScancode((tank + k)->right) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->right)), rback,
                           gback, bback, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] -= 250;
                vy[z] -= 45;
            }
            x -= 250;
            y -= 45;

            yes = i % 6 == 4 && k == i / 6;
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
            if ((tank + k)->shoot) {
                if (SDL_GetKeyFromScancode((tank + k)->shoot) > 200) x1 = x + 2;
                else x1 = x + 20;
                stringRGBA(renderer, x1, y + 20, SDL_GetKeyName(SDL_GetKeyFromScancode((tank + k)->shoot)), rback,
                           gback, bback, a);
            }

            int x2 = MAP_WIDTH / 2 - house;
            int y2 = (k + 1) * MAP_HEIGHT / 3 + 3 * house / 2;
            yes = i % 6 == 5 && k == i / 6;
            stringRGBA(renderer, x2, y2, "Name:", redj[yes], greenj[yes], bluej[yes], a);
            if (yes || enter) {
                static int f[numberofTanks] = {0};
                static int z = 0;
                if (enter) {
                    f[k] = 0;
                    z = 0;
                }
                if (!enter) {
                    if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode &&
                        *SDL_GetKeyName(keycode) != '') {
                        if (keycode == SDLK_BACKSPACE && z > 0) {
                            static int p = 0;
                            if (p == 0) {
                                z--;
                                (tank + k)->name[z] = 0;
                            }
                            p++;
                            if (p == 2) p = 0;
                        } else {
                            (tank + k)->name[z] = *SDL_GetKeyName(keycode) * (z == 0) +
                                                  (*SDL_GetKeyName(keycode) - 'A' + 'a') * (z != 0);
                            if (z < 9) z++;
                            f[k] = 1;
                        }
                    }
                    if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && (tank + k)->name[0] && f[k]) {
                        i++;
                        z = 0;
                    }
                }
            }
            if ((tank + k)->name[0]) {
                int x1 = MAP_WIDTH / 2;
                int y1 = (k + 1) * MAP_HEIGHT / 3 + 3 * house / 2;
                stringRGBA(renderer, x1, y1, (tank + k)->name, (tank + k)->r, (tank + k)->g, (tank + k)->b, a);
            }

            for (int z = 0; z < 4; z++) {
                vx[z] += -90 + 250;
                vy[z] += 45 + 275;
            }
            x += 250 - 90;
            y += 45 + 275;
            if (i >= 6) j = tank2;
        }
        if (i == 12) break;
        enter = 0;
        if (keycodepre != keycode && keycode) keycodepre = keycode;
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    tank->bullets = bullet;
    (tank + 1)->bullets = bullet + numberofBullets;
    map->tanks = tank;
    map->walls = walls;

    ///////////////for reading from file. it works correctly
    //read_map_file(map, "D:\\programming\\c\\University\\project\\project\\src\\map.txt");

    read_map_array(map);

    for (int i = 0; i < 2; i++) {
        (tank + i)->x = rand() % numberofColumns * house + house;
        (tank + i)->y = rand() % numberofRows * house + house;
        (tank + i)->angle = (rand() % 360) * M_PI / 180;
        (tank + i)->boolian = true;
        (tank + i)->score = 0;
        (tank + i)->fragBomb = 0;
        (tank + i)->mine = 0;
        (tank + i)->lazer = 0;
        (tank + i)->item = false;
    }

    while (tank->x == (tank + 1)->x && tank->y == (tank + 1)->y)
        (tank + 1)->y = rand() % numberofColumns * house + house;

    for (int i = 0; i < numberofBullets; i++) {
        (bullet + i)->boolian = true;
        (bullet + i)->x = -100;
        (bullet + i)->y = -100;
        (bullet + i)->n = 0;
        (bullet + i)->rad = radius_bullet;
        (bullet + numberofBullets + i)->boolian = true;
        (bullet + numberofBullets + i)->x = -100;
        (bullet + numberofBullets + i)->y = -100;
        (bullet + numberofBullets + i)->n = 0;
        (bullet + numberofBullets + i)->rad = radius_bullet;
    }
    remaining = numberofTanks;

    start_flag = true;
    whilePlayingMenu = true;
    return flag;
}


bool menu(Tank *tank, Bullet *bullet, Map *map, Wall *walls, bool flag) {
    int width = MAP_WIDTH, height = MAP_HEIGHT;
    bool f = flag;
    Tank *sample1 = malloc(sizeof(Tank));
    Tank *sample2 = malloc(sizeof(Tank));
    if (flag) {
        MAP_WIDTH = 800, MAP_HEIGHT = 800;
        sample1->x = MAP_WIDTH / 2;
        sample1->y = 750;
        sample1->angle = 0;
        sample1->r = 180;
        sample1->g = 180;
        sample1->b = 180;
        sample2->x = MAP_WIDTH / 2;
        sample2->y = MAP_HEIGHT - sample1->y;
        sample2->angle = M_PI;
        sample2->r = 180;
        sample2->g = 180;
        sample2->b = 180;
    }
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
            if (n >= 5) {
                MAP_HEIGHT = height;
                MAP_WIDTH = width;
                flag = false;
            }
        }

        red[j] = 100;
        green[j] = 100;
        blue[j] = 100;
        if (flag) {
            SDL_RenderSetScale(renderer, 1.5, 1.5);
            draw_tank(sample1);
            draw_tank(sample2);
            int x = (MAP_WIDTH / 2 - house / 2 - 10) / 1.5;
            int y = 300 / 1.5;
            stringRGBA(renderer, x, y, "Start Game", red[new], green[new], blue[new], a);
            x += 4;
            y += 25;
            stringRGBA(renderer, x, y, "Load Game", red[load], green[load], blue[load], a);
            x += 3;
            y += 25;
            stringRGBA(renderer, x, y, "End Game", red[end], green[end], blue[end], a);
            SDL_RenderSetScale(renderer, 1, 1);
        }
        sample1->x += step * cos(sample1->angle);
        sample1->y += step * sin(sample1->angle);
        sample1->angle -= 0.1;
        if (sample1->x < 0) sample1->x = (MAP_WIDTH - radius_circle) / 1.5;
        if (sample1->y < 0) sample1->y = (MAP_HEIGHT - radius_circle) / 1.5;
        if (sample1->x > MAP_WIDTH / 1.5) sample1->x = radius_circle;
        if (sample1->y > MAP_HEIGHT / 1.5) sample1->y = radius_circle;
        sample2->angle -= 0.1;
        sample2->x = MAP_WIDTH / 1.5 - sample1->x;
        sample2->y = MAP_HEIGHT / 1.5 - sample1->y;
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }
    if (f != flag) SDL_Delay(200);
    return flag;
}

int numberofchars(char *name) {
    int i = 0;
    while (name[i] != 0) i++;
    return i;
}

int argham(int n) {
    int m = n / 10, ragham = 1;
    while (m / 10) {
        ragham++;
        m /= 10;
    }
    return ragham;
}

void makingstring(int n, char *score, int ragham) {
    int i = 0;
    *(score + ragham) = 0;
    while (ragham) {
        *(score + ragham - 1) = n % 10 + '0';
        n /= 10;
        ragham--;
    }
}

int isEqual(char *c1, char *c2, int n) {
    if (n == 0) return 1;
    if (*c1 != *c2 || numberofchars(c1) - 1 != numberofchars(c2)) return 0;
    return isEqual(c1 + 1, c2 + 1, n - 1);
}

void copy(char *c, char *b) {
    int i = 0;
    while (b[i]) {
        c[i] = b[i];
        i++;
    }
}

void saveGame(Tank *tank, Bullet *bullet, Wall *walls, Item *item, Shard *shard, Mine *mine, char *fileName) {
    char name[100] = {0};
    char c = 0;
    int n;
    bool flag = true;
    FILE *file1 = fopen("D:\\programming\\c\\University\\project\\project\\src\\numberofFileNames", "r");
    fscanf(file1, "%d", &n);
    fclose(file1);
    file1 = fopen("D:\\programming\\c\\University\\project\\project\\src\\fileNames.txt", "r");
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (c != '\n') {
            fscanf(file1, "%c", &c);
            name[j] = c;
            j++;
        }
        if (isEqual(name + 48, fileName, numberofchars(fileName))) flag = false;
        for (j = 0; j < 48 + numberofchars(fileName); j++) name[j] = 0;
        c = 0;
    }
    fclose(file1);
    if (flag) {
        file1 = fopen("D:\\programming\\c\\University\\project\\project\\src\\fileNames.txt", "a");
        fprintf(file1, "D:\\programming\\c\\University\\project\\project\\src\\");
        fputs(fileName, file1);
        fprintf(file1, "\n");
        fclose(file1);
    }
    if (flag) {
        file1 = fopen("D:\\programming\\c\\University\\project\\project\\src\\numberofFileNames", "w");
        fprintf(file1, "%d", n + 1);
        fclose(file1);
    }
    copy(name, "D:\\programming\\c\\University\\project\\project\\src\\");
    copy(name + 48, fileName);
    copy(name + 48 + numberofchars(fileName), ".txt");
    printf("%s", name);
    file1 = fopen(name, "w+");
    fprintf(file1, "winnerScore: %d", winnerScore);
    fprintf(file1, "numberofColumns: %d\n", numberofColumns);
    fprintf(file1, "numberofRows: %d\n", numberofRows);
    for (int i = 0; i < (numberofRows + 1) * numberofRows + (numberofRows + 1) * numberofColumns; i++)
        fprintf(file1, "wall%d: %d\n", i, (walls + i)->boolian);
    for (int i = 0; i < numberofTanks; i++) {
        fprintf(file1, "tank%d->x: %d\n", i, (tank + i)->x);
        fprintf(file1, "tank%d->y: %d\n", i, (tank + i)->y);
        fprintf(file1, "tank%d->r: %d\n", i, (tank + i)->r);
        fprintf(file1, "tank%d->g: %d\n", i, (tank + i)->g);
        fprintf(file1, "tank%d->b: %d\n", i, (tank + i)->b);
        fprintf(file1, "tank%d->right: %d\n", i, (tank + i)->right);
        fprintf(file1, "tank%d->left: %d\n", i, (tank + i)->left);
        fprintf(file1, "tank%d->up: %d\n", i, (tank + i)->up);
        fprintf(file1, "tank%d->down: %d\n", i, (tank + i)->down);
        fprintf(file1, "tank%d->shoot: %d\n", i, (tank + i)->shoot);
        fprintf(file1, "tank%d->name: %s\n", i, (tank + i)->name);
        fprintf(file1, "tank%d->score: %d\n", i, (tank + i)->score);
        fprintf(file1, "tank%d->boolian: %d\n", i, (tank + i)->boolian);
        fprintf(file1, "tank%d->item: %d\n", i, (tank + i)->item);
        fprintf(file1, "tank%d->fragBomb: %d\n", i, (tank + i)->fragBomb);
        fprintf(file1, "tank%d->mine: %d\n", i, (tank + i)->mine);
        fprintf(file1, "tank%d->lazer: %d\n", i, (tank + i)->lazer);
        fprintf(file1, "tank%d->lazerTime: %d\n", i, (tank + i)->lazerTime);
        fprintf(file1, "tank%d->angle %f\n", i, (tank + i)->angle);
    }

    for (int i = 0; i < numberofTanks * numberofBullets; i++) {
        fprintf(file1, "bullet%d->x: %d\n", i, (bullet + i)->x);
        fprintf(file1, "bullet%d->y: %d\n", i, (bullet + i)->y);
        fprintf(file1, "bullet%d->angle: %f\n", i, (bullet + i)->angle);
        fprintf(file1, "bullet%d->n: %d\n", i, (bullet + i)->n);
        fprintf(file1, "bullet%d->rad: %d\n", i, (bullet + i)->rad);
        fprintf(file1, "bullet%d->boolian: %d\n", i, (bullet + i)->boolian);
    }

    for (int i = 0; i < numberofItems; i++) {
        fprintf(file1, "item%d->x: %d\n", i, (item + i)->x);
        fprintf(file1, "item%d->y: %d\n", i, (item + i)->y);
        fprintf(file1, "item%d->n: %d\n", i, (item + i)->n);
        fprintf(file1, "item%d->type: %d\n", i, (item + i)->type);
        fprintf(file1, "item%d->boolian: %d\n", i, (item + i)->boolian);
    }

    for (int i = 0; i < numberofItems; i++) {
        fprintf(file1, "mine%d->x: %d\n", i, (mine + i)->x);
        fprintf(file1, "mine%d->y: %d\n", i, (mine + i)->y);
        fprintf(file1, "mine%d->r: %d\n", i, (mine + i)->r);
        fprintf(file1, "mine%d->g: %d\n", i, (mine + i)->g);
        fprintf(file1, "mine%d->b: %d\n", i, (mine + i)->b);
        fprintf(file1, "mine%d->n: %d\n", i, (mine + i)->n);
        fprintf(file1, "mine%d->boolian: %d\n", i, (mine + i)->boolian);
    }

    for (int i = 0; i < numberofTanks * numberofShards; i++) {
        fprintf(file1, "shard%d->x: %d\n", i, (shard + i)->x);
        fprintf(file1, "shard%d->y: %d\n", i, (shard + i)->y);
        fprintf(file1, "shard%d->angle: %f\n", i, (shard + i)->angle);
        fprintf(file1, "shard%d->boolian: %d\n", i, (shard + i)->boolian);
    }
    fclose(file1);
}

int main(int argc, char *argv[]) {

    srand(time(0));
    Bullet *bullet = malloc(numberofTanks * sizeof(Bullet) * numberofBullets);
    Tank *tank = malloc(sizeof(Tank) * numberofTanks);
    Tank *sample = malloc(sizeof(Tank) * numberofTanks);
    Map *map = malloc(sizeof(Map) * 1);
    Wall *walls;
    Item *item = malloc(sizeof(Item) * numberofItems);
    Shard *shard = malloc(numberofTanks * sizeof(Shard) * numberofShards);
    Mine *mine = malloc(sizeof(Mine) * numberofTanks);

    //////////////////menu
    init_window();
    bool flag = true;
    char score[5];

    for (int j = 0; j < 5; j++) score[j] = 0;

    char fileName[10];

    for (int j = 0; j < 10; j++) fileName[j] = 0;

    while (1) {
        int red = 150, green = 150, blue = 150;
        if (rback == 225) red = red_white - red, green = green_white - green, blue = blue_white - blue;
        flag = menu(tank, bullet, map, walls, flag);
        static int time = 0;
        if (!whilePlayingMenu) time++;

        if (start_flag) {
            for (int i = 0; i < numberofItems; i++) {
                (item + i)->boolian = false;
                (mine + i)->boolian = false;
            }
            for (int i = 0; i < numberofTanks * numberofShards; i++) {
                (shard + i)->boolian = false;
            }
        }
        SDL_Keycode keycode = 0;
        keycode = handle_events(map);
        for (int i = 0; i < numberofWalls; i++)
            if ((map->walls + i)->boolian)
                draw_walls(map->walls + i);

        if (time == timeofItem && !whilePlayingMenu) {
            time = 0;
            int i = 0;
            for (int j = 0; j < 3; j++)
                if ((item + j)->boolian)
                    i++;

            if (i < 3) {
                for (int j = 0; j < 3; j++)
                    if (!(item + j)->boolian) {
                        (item + j)->n = 0;
                        (item + j)->x = rand() % numberofColumns * house + house;
                        (item + j)->y = rand() % numberofRows * house + house;
                        (item + j)->boolian = true;
                        int k = rand() % 3;
                        (item + j)->type = k;
                        break;
                    }
            }
        }

        for (int i = 0; i < numberofItems; i++) {
            if ((item + i)->boolian) {
                draw_item(item + i);
                if ((item + i)->n == timeofItem) (item + i)->boolian = false;
                for (int k = 0; k < numberofTanks; k++)
                    if ((map->tanks + k)->boolian)
                        tank_collids_Items(item + i, map->tanks + k);
            }
        }

        int x = MAP_WIDTH - 75, y = MAP_HEIGHT / 3;
        for (int k = 0; k < numberofTanks; k++) {
            if ((mine + k)->boolian) {
                draw_mine(mine + k, shard, map->tanks);
                for (int i = 0; i < numberofTanks; i++)
                    if ((map->tanks + i)->boolian)
                        tank_collids_mine(mine + k, map->tanks + i);
            }
            (sample + k)->angle = -M_PI / 2;
            (sample + k)->x = x;
            (sample + k)->y = y;
            (sample + k)->r = (map->tanks + k)->r;
            (sample + k)->g = (map->tanks + k)->g;
            (sample + k)->b = (map->tanks + k)->b;
            if ((map->tanks + k)->boolian) {
                if (!whilePlayingMenu) move_tank(map->tanks + k, map);
                if (!whilePlayingMenu) turn_tank(map->tanks + k, map);
                draw_tank(map->tanks + k);
                draw_lazer(tank + k, tank + numberofTanks - 1 - k);
            }
            for (int i = 0; i < numberofBullets; i++) {
                bullet_collids_walls((map->tanks + k)->bullets + i, map);
                move_bullet((map->tanks + k)->bullets + i);
                draw_bullet((map->tanks + k)->bullets + i);
                bullet_collids_tanks((map->tanks + k)->bullets + i, map->tanks, shard);
            }
            draw_tank(sample + k);
            y = MAP_HEIGHT / 3 + radius_circle + 15;
            stringRGBA(renderer, x - numberofchars(tank->name) * 4, y, tank->name, red, green, blue, a);
            char *score1, *score2;
            int ragham1 = argham(tank->score), ragham2 = argham((tank + 1)->score);
            score1 = malloc(ragham1 * sizeof(char));
            score2 = malloc(ragham2 * sizeof(char));
            makingstring(tank->score, score1, ragham1);
            makingstring((tank + 1)->score, score2, ragham2);
            y += 15;
            stringRGBA(renderer, x - (ragham1) * 3, y, score1, red, green, blue, a);
            y = 2 * MAP_HEIGHT / 3 + radius_circle + 15;
            stringRGBA(renderer, x - numberofchars((tank + 1)->name) * 4, y, (tank + 1)->name, red, green, blue, a);
            y += 15;
            stringRGBA(renderer, x - (ragham2) * 3, y, score2, red, green, blue, a);
            y -= 30 + radius_circle;
            free(score1);
            free(score2);
        }
        if (!whilePlayingMenu) fire(map->tanks, shard, mine);

        for (int i = 0; i < numberofTanks * numberofShards; i++) {
            if ((shard + i)->boolian) {
                draw_shard(shard + i);
                move_shard(shard + i, map);
                for (int l = 0; l < 2; l++)
                    if ((map->tanks + l)->boolian)
                        shard_collids_tanks(shard + i, map->tanks + l);
            }
        }

        if (whilePlayingMenu) {
            static int z = 0;
            static int f = 0;
            short vx[] = {MAP_WIDTH / 2 - 5 * house / 2, MAP_WIDTH / 2 - 5 * house / 2,
                          MAP_WIDTH / 2 + 5 * house / 2, MAP_WIDTH / 2 + 5 * house / 2};
            short vy[] = {MAP_HEIGHT / 2 - 4 * house / 2, MAP_HEIGHT / 2 + 4 * house / 2,
                          MAP_HEIGHT / 2 + 4 * house / 2, MAP_HEIGHT / 2 - 4 * house / 2};
            filledPolygonRGBA(renderer, vx, vy, 4, red_white - rback, green_white - gback, blue_white - bback, 220);

            if (saveGame_flag) {
                stringRGBA(renderer, MAP_WIDTH / 2 - numberofchars("Give me the name for saving the file:") * 4,
                           MAP_HEIGHT / 2 - house / 4, "Give me the name for saving the file:", rback, gback, bback, a);

                if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode &&
                    *SDL_GetKeyName(keycode) != '') {
                    if (keycode == SDLK_BACKSPACE && z > 0) {
                        static int p = 0;
                        if (p == 0) {
                            z--;
                            fileName[z] = 0;
                        }
                        p++;
                        if (p == 2) p = 0;
                    } else {
                        fileName[z] = *SDL_GetKeyName(keycode);
                        if (z < 9) z++;
                        f = 1;
                    }
                }
                if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && fileName[0] && f) {
                    saveGame(map->tanks, map->tanks->bullets, map->walls, item, shard, mine, fileName);
                    whilePlayingMenu = false;
                    for (int j = 0; j < 10; j++) fileName[j] = 0;
                    f = 0;
                    z = 0;
                }

                if (fileName[0]) {
                    int x1 = MAP_WIDTH / 2;
                    int y1 = MAP_HEIGHT / 2 + house / 4;
                    stringRGBA(renderer, x1 - numberofchars(fileName) * 3, y1, fileName, rback, gback, bback, a);
                }
            } else if (winner) {
                int red = rand() % 255;
                int green = rand() % 255;
                int blue = rand() % 255;
                stringRGBA(renderer, MAP_WIDTH / 2 - numberofchars((tank + winner - 1)->name) * 4 -
                                     numberofchars(" won the game!") * 4, MAP_HEIGHT / 2 - house / 4,
                           (tank + winner - 1)->name, red, green, blue, a);
                stringRGBA(renderer, MAP_WIDTH / 2 - numberofchars(" won the game!") * 2, MAP_HEIGHT / 2 - house / 4,
                           " won the game!", red, green, blue, a);
                stringRGBA(renderer, MAP_WIDTH / 2 - numberofchars("Press Enter to continue") * 4,
                           MAP_HEIGHT / 2 + house / 4, "Press Enter to continue", red, green, blue, a);
                if (state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) {
                    whilePlayingMenu = false;
                    winner = 0;
                    winnerScore = 0;
                    newGame(tank, bullet, map, walls, true);
                    for (int i = 0; i < numberofchars(score); i++) score[i] = 0;
                    z = 0;
                    f = 0;
                }
            } else {
                stringRGBA(renderer, MAP_WIDTH / 2 - numberofchars("Give me winner's score!") * 4,
                           MAP_HEIGHT / 2 - house / 4, "Give me winner's score!", rback, gback, bback, a);

                if (!state[SDL_SCANCODE_RETURN] && keycode != SDLK_RETURN && keycode &&
                    *SDL_GetKeyName(keycode) != '') {
                    if (keycode == SDLK_BACKSPACE && z > 0) {
                        static int p = 0;
                        if (p == 0) {
                            z--;
                            score[z] = 0;
                        }
                        p++;
                        if (p == 2) p = 0;
                    } else if (*SDL_GetKeyName(keycode) >= '0' && *SDL_GetKeyName(keycode) <= '9') {
                        score[z] = *SDL_GetKeyName(keycode);
                        if (z < 3) z++;
                        f = 1;
                    }
                }
                if ((state[SDL_SCANCODE_RETURN] || keycode == SDLK_RETURN) && score[0] && f) {
                    whilePlayingMenu = false;
                    winnerScore = 0;
                    f = 0;
                    z = 0;
                    for (int i = 0; i < numberofchars(score); i++) winnerScore = winnerScore * 10 + score[i] - '0';
                    printf("%d\n", winnerScore);
                }

                if (score[0]) {
                    int x1 = MAP_WIDTH / 2;
                    int y1 = MAP_HEIGHT / 2 + house / 4;
                    stringRGBA(renderer, x1 - numberofchars(score) * 3, y1, score, rback, gback, bback, a);
                }
            }
        }

        if (remaining == 1) {
            static int n = 0;
            n++;
            if (n == 200) {
                n = 0;
                for (int i = 0; i < numberofTanks; i++) {
                    if ((tank + i)->boolian) {
                        (tank + i)->score++;
                        if ((tank + i)->score == winnerScore) {
                            remaining = numberofTanks;
                            winner = i + 1;
                            whilePlayingMenu = true;
                        } else nextGame(map->tanks, map->tanks->bullets, map, map->walls);
                        break;
                    }
                }
            }
        }

        if (remaining == 0) {
            static int n = 0;
            n++;
            if (n == 10) {
                n = 0;
                nextGame(map->tanks, map->tanks->bullets, map, map->walls);
            }
        }

        if ((state[224] || state[228]) && state[SDL_SCANCODE_S]) {
            whilePlayingMenu = true;
            saveGame_flag = true;
        }

        if (state[SDL_SCANCODE_ESCAPE]) {
            flag = true;
            SDL_Delay(200);
        }
        //if(state[tank->shoot]) printf("%d\n", state[tank->shoot]);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FPS);
    }
}