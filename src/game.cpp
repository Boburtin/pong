#include <algorithm>

#include "Pong.h"

void update(Ball &ball, Paddle &paddle, float dt, std::bitset<256> &keys) {
    
    ball.x += ball.vx * dt;
    ball.y += ball.vy * dt;
    paddle.y += paddle.vy * dt;
    
    float dx = ball.x - std::clamp(ball.x, paddle.rect().left, paddle.rect().right);
    float dy = ball.y - std::clamp(ball.y, paddle.rect().top, paddle.rect().bottom);
    
    float dx2 = dx * dx, dy2 = dy * dy;
    
    if (dx2 + dy2 <= ball.radius2) {
        if (dx2 > dy2) ball.vx = -ball.vx;
        else if (dy2 > dx2) ball.vy = -ball.vy;
        else ball.vx = -ball.vx, ball.vy = -ball.vy;
        
        float dist = std::sqrt(dx2 + dy2);
        
        if (dist > 0) {
            
            float nx = dx / dist, ny = dy / dist;

            ball.x = std::clamp(ball.x, paddle.rect().left, paddle.rect().right) + nx * ball.radius;
            ball.y = std::clamp(ball.y, paddle.rect().top, paddle.rect().bottom) + ny * ball.radius;
        }
        else {
            float to_left = ball.x - paddle.rect().left;
            float to_right = ball.x - paddle.rect().right;
            if (to_left < to_right) ball.x = paddle.rect().left - ball.radius;
            else ball.x = paddle.rect().right + ball.radius;
        }
    }
    float overlap_dx = ball.x - std::clamp(ball.x, ball.radius, WIDTH - ball.radius);
    float overlap_dy = ball.y - std::clamp(ball.y, ball.radius, HEIGHT - ball.radius);
    float overlap_dx2 = overlap_dx * overlap_dx, overlap_dy2 = overlap_dy * overlap_dy;

    if (overlap_dx2 + overlap_dy2 > 0) {
        if (overlap_dx2 > overlap_dy2) ball.vx = -ball.vx;
        else if (overlap_dy2 > overlap_dx2) ball.vy = -ball.vy;
        else ball.vx = -ball.vx, ball.vy = -ball.vy;

        ball.x = std::clamp(ball.x, ball.radius, WIDTH - ball.radius);
        ball.y = std::clamp(ball.y, ball.radius, HEIGHT - ball.radius);
    }
    paddle.vy = keys['W'] && paddle.rect().top > 0               ? -SPEED
                : keys['S'] && paddle.rect().bottom < HEIGHT - 1 ? SPEED
                                                                 : 0;
}

void render(const Ball &ball, const Paddle &paddle, RenderContext &rc) {
    using namespace D2D1;
    rc.pRT->BeginDraw();
    rc.pRT->Clear(ColorF(0.f, 0.f, 0.f, 1.f));
    rc.pBrush->SetColor(ColorF(0.f, .5f, 0.f, 1.f));
    rc.pRT->FillEllipse(ball.ellipse(), *rc.pBrush.GetAddressOf());
    rc.pBrush->SetColor(ColorF(ColorF::White));
    rc.pRT->FillRectangle(paddle.rect(), *rc.pBrush.GetAddressOf());
    rc.pRT->EndDraw();
}