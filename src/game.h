#ifndef GAME_H
#define GAME_H

#include <bitset>
#include <d2d1.h>

inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 600;
inline constexpr float PADDLE_W = 40.f;
inline constexpr float PADDLE_H = 120.f;
inline constexpr double SPEED = 600.f;

struct Ball
{
    float x{700.f}, y{300.f};
    float vx, vy;
    float radius{20.f};
    float radius2{400.f};
    D2D1_ELLIPSE
    ellipse() const
    {
        return {x, y, radius, radius};
    }
    Ball(float vx, float vy) : vx(vx), vy(vy) {};
};

struct Paddle
{
    float x{0.f}, y{240.f};
    float vy{0.f};
    D2D1_RECT_F
    rect() const
    {
        return {x, y, x + PADDLE_W, y + PADDLE_H};
    }
};

void update(Ball &ball, Paddle &paddle, float dt, std::bitset<256> &keys);

#endif