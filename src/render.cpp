#include "render.h"
#include "game.h"
#include <windows.h>

void InitRenderContext(RenderContext &rc, HWND hwnd) {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, rc.pFactory.GetAddressOf());
    RECT rect {};
    GetClientRect(hwnd, &rect);
    D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
    rc.pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                        D2D1::HwndRenderTargetProperties(hwnd, size),
                                        rc.pRT.GetAddressOf());
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                        reinterpret_cast<IUnknown **>(rc.pDWFactory.GetAddressOf()));
    rc.pRT->CreateSolidColorBrush(D2D1::ColorF(.1f, .1f, .1f, .1f),
                                  rc.pBrush.GetAddressOf());
    rc.pDWFactory->CreateTextFormat(L"Consolas", nullptr, DWRITE_FONT_WEIGHT_MEDIUM,
                                    DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
                                    20.f, L"en-us", rc.pHUDFont.GetAddressOf());
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
