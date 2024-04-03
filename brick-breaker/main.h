#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

struct Ball {
    int row;
    int col;
    int rd;
    int cd;
    int radius;
    unsigned short color;
};

struct Paddle {
    int row;
    int col;
    int width;
    int height;
    int speed;
};

struct Brick {
    int row;
    int col;
    int width;
    int height;
};

// int sc;

#endif
