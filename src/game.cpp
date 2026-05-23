#include "game.h"
#include <algorithm>
#include <bitset>
#include <cmath>

void update(Ball &ball, Paddle &paddle, float dt, std::bitset<256> &keys) {
    /* Velocity helper */
    auto scale_speed = [](Ball &ball, float factor) {
        float speed = std::sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
        /* Basically useless guard against division by zero */
        if (speed <= 0.f) return;
        /* Clamp between floor and ceiling speed */
        float new_speed = std::clamp(speed * factor, MIN_SPEED, MAX_SPEED);
        /* Acceleration value */
        float ratio = new_speed / speed;
        ball.vx *= ratio;
        ball.vy *= ratio;
    };
    /* Magnus effect */
    float dtheta = ball.omega * dt;
    float cs = std::cos(dtheta);
    float sn = std::sin(dtheta);
    float new_vx = cs * ball.vx - sn * ball.vy;
    float new_vy = sn * ball.vx + cs * ball.vy;
    ball.vx = new_vx;
    ball.vy = new_vy;
    ball.omega *= std::exp(-SPIN_DECAY * dt);
    /* Ball position update */
    ball.x += ball.vx * dt;
    ball.y += ball.vy * dt;
    /* Paddle position update */
    paddle.y += paddle.vy * dt;
    /* Paddle/Ball collision */
    float dx = ball.x - std::clamp(ball.x, paddle.rect().left, paddle.rect().right);
    float dy = ball.y - std::clamp(ball.y, paddle.rect().top, paddle.rect().bottom);
    float dx2 = dx * dx;
    float dy2 = dy * dy;
    /* Distance <= radius squared means collision */
    if (dx2 + dy2 <= ball.radius2) {
        if (dx2 > dy2) {
            /* The closest point to the ball is the right face */
            ball.vx = -ball.vx;
        } else if (dy2 > dx2) {
            /* The closest point to the ball is the top/bottom face */
            ball.vy = -ball.vy;
        } else {
            /* Corner hit */
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        ball.omega += paddle.vy * SPIN_TRANSFER;
        scale_speed(ball, HIT_BOOST);
        float dist = std::sqrt(dx2 + dy2);
        if (dist > 0) {
            float nx = dx / dist;
            float ny = dy / dist;
            ball.x = std::clamp(ball.x, paddle.rect().left, paddle.rect().right) + nx * ball.radius;
            ball.y = std::clamp(ball.y, paddle.rect().top, paddle.rect().bottom) + ny * ball.radius;
        } else {
            float to_left = ball.x - paddle.rect().left;
            float to_right = ball.x - paddle.rect().right;
            if (to_left < to_right) {
                ball.x = paddle.rect().left - ball.radius;
            } else ball.x = paddle.rect().right + ball.radius;
        }
    }
    /* Wall checks */
    float wall_overlap_dx = ball.x - std::clamp(ball.x, ball.radius, WIDTH - ball.radius);
    float wall_overlap_dy = ball.y - std::clamp(ball.y, ball.radius, HEIGHT - ball.radius);
    float wall_overlap_dx2 = wall_overlap_dx * wall_overlap_dx;
    float wall_overlap_dy2 = wall_overlap_dy * wall_overlap_dy;
    if (wall_overlap_dx2 + wall_overlap_dy2 > 0) {
        if (wall_overlap_dx2 > wall_overlap_dy2) {
            ball.vx = -ball.vx;
            /* Ball overlaps left or right walls */
        } else if (wall_overlap_dy2 > wall_overlap_dx2) {
            /* Ball overlaps top or bottom walls */
            ball.vy = -ball.vy;
        } else {
            /* Perfect corner overlap */
            ball.vx = -ball.vx;
            ball.vy = -ball.vy;
        }
        /* Clamp to in-bounds */
        ball.x = std::clamp(ball.x, ball.radius, WIDTH - ball.radius);
        ball.y = std::clamp(ball.y, ball.radius, HEIGHT - ball.radius);
        /* Update velocity */
        scale_speed(ball, WALL_DRAG);
    }
    if (keys['W'] && paddle.rect().top > 0) {
        paddle.vy = -SPEED;
    } else if (keys['S'] && paddle.rect().bottom < HEIGHT - 1) {
        paddle.vy = SPEED;
    } else paddle.vy = 0;
}
