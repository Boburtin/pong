#ifndef CONSTANT_H
#define CONSTANT_H

inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 600;
inline constexpr float PADDLE_W = 40.f;
inline constexpr float PADDLE_H = 120.f;
inline constexpr double SPEED = 600.f;
inline constexpr float MIN_SPEED = 400.f;
inline constexpr float MAX_SPEED = 1200.f;
inline constexpr float HIT_BOOST = 1.1f;
inline constexpr float WALL_DRAG = 0.99f;
inline constexpr float SPIN_DECAY = 1.f;
inline constexpr float SPIN_TRANSFER = .001f;

#endif