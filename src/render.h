#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

struct RenderContext
{
    Microsoft::WRL::ComPtr<ID2D1Factory> pFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRT;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
    Microsoft::WRL::ComPtr<IDWriteFactory> pDWFactory;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> pHUDFont;
    bool fullRedraw = false;
};

void render(const Ball &ball, const Paddle &paddle, RenderContext &rc);

#endif