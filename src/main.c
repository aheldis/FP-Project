#include "structs.h"

#ifdef main
#undef main
#endif


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
            if (abs(connectedVertex - vertex) == 1)  (walls + i % numberofColumns + i / numberofColumns * (numberofColumns + 1))->boolian = false;
            else  (walls + i + numberofRows * (numberofColumns + 1))->boolian = false;
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
            if (rend == 1 && j != 0 && j != numberofColumns)   (map->walls + n)->boolian = false;
            else  (map->walls + n)->boolian = true;
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
            if(rend == 1 && i != 0 && i != numberofRows)    (map->walls + n)->boolian = false;
            else  (map->walls + n)->boolian = true;
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

/*void read_map_file(Map *map, char *file_path) {
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
}*/


int main(int argc, char *argv[]) {
    Bullet *bullet = malloc(sizeof(Bullet) * numberofBullets);
    Tank *tank_1 = malloc(sizeof(Tank) * 1);
    Map *map = malloc(sizeof(Map) * 1);
    Wall *walls = malloc(sizeof(Wall) * numberofWalls);

    srand(time(0));
    tank_1->x = rand() % numberofRows * house + house;
    tank_1->y = rand() % numberofColumns * house + house;
    tank_1->r = rand() % 255;
    tank_1->g = rand() % 255;
    tank_1->b = rand() % 255;
    tank_1->angle = rand() % 360;

    bullet->boolian = false;
    tank_1->bullets = bullet;
    map->tanks = tank_1;
    map->walls = walls;

    ///////////////for reading from file. it works correctly
    //read_map_file(map, "D:\\programming\\c\\University\\project\\project\\src\\map.txt");

    read_map_array(map);

    init_window();
    while (1) {
        handle_events(map);
        for (int i = 0; i < numberofWalls; i++)
            if ((map->walls + i)->boolian)
                draw_walls(map->walls + i);
        for (int i = 0; i < numberofBullets; i++) draw_bullet(map->tanks->bullets + i);
        draw_tank(map->tanks);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FPS);
    }
    quit_window();
}