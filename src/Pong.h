#ifndef PONG_H
#define PONG_H

#include <d2d1.h>
#include <dwrite.h>
#include <intrin.h>
#include <windows.h>
#include <wrl/client.h>

#include <bitset>
#include <cmath>

inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 600;
inline constexpr float PADDLE_W = 40.f;
inline constexpr float PADDLE_H = 120.f;
inline constexpr double SPEED = 600.f;

struct RenderContext {
    Microsoft::WRL::ComPtr<ID2D1Factory> pFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRT;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
    Microsoft::WRL::ComPtr<IDWriteFactory> pDWFactory;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> pHUDFont;
    bool fullRedraw = false;
};

struct Ball {
    float x {700.f}, y {300.f};
    float vx, vy;
    float radius {20.f};
    float radius2 {400.f};
    D2D1_ELLIPSE ellipse() const { return {x, y, radius, radius}; }
    Ball(float vx, float vy) : vx(vx), vy(vy) {};
};

struct Paddle {
    float x, y;
    float vy;
    D2D1_RECT_F rect() const { return {x, y, x + PADDLE_W, y + PADDLE_H}; }
};

void InitRenderContext(RenderContext &rc, HWND hwnd);

void update(Ball &ball, Paddle &paddle, float dt, std::bitset<256> &keys);

void render(const Ball &ball, const Paddle &paddle, RenderContext &rc);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif