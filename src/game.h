#ifndef GAME_H
#define GAME_H

#include "Constant.h"
#include <bitset>
#include <d2d1.h>

struct Ball {
    float x {700.f}, y {300.f};
    float vx, vy;
    float omega {0.f};
    float radius {20.f};
    float radius2 {400.f};
    Ball(float vx, float vy) : vx(vx), vy(vy) {};
    D2D1_ELLIPSE ellipse() const { return {x, y, radius, radius}; }
};

struct Paddle {
    float x {0.f}, y {240.f};
    float vy {0.f};
    D2D1_RECT_F rect() const { return {x, y, x + PADDLE_W, y + PADDLE_H}; }
};

void update(Ball &ball, Paddle &paddle, float dt, std::bitset<256> &keys);

#endif