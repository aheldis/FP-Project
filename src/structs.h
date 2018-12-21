#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H
typedef struct {
    int x, y;
    double angle;
    bool boolian;
} Bullet;

typedef struct {
    int x;
    int y;
    int r;
    int g;
    int b;
    double angle;
    Bullet *bullets;
} Tank;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Wall;

typedef struct {
    Tank *tanks;
    Wall *walls;
} Map;

#endif //PROJECT_STRUCTS_H
