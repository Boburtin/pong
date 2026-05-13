#include <numbers>
#include <random>

#include "Pong.h"

void InitRenderContext(RenderContext &rc, HWND hwnd) {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, rc.pFactory.GetAddressOf());
    RECT rect;
    GetClientRect(hwnd, &rect);
    D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
    rc.pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size),
                                        rc.pRT.GetAddressOf());
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                        reinterpret_cast<IUnknown **>(rc.pDWFactory.GetAddressOf()));
    rc.pRT->CreateSolidColorBrush(D2D1::ColorF(.1f, .1f, .1f, .1f), rc.pBrush.GetAddressOf());
    rc.pDWFactory->CreateTextFormat(L"Consolas", nullptr, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL,
                                    DWRITE_FONT_STRETCH_NORMAL, 20.f, L"en-us", rc.pHUDFont.GetAddressOf());
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angle_dis(30.f, 60.f);
    std::uniform_int_distribution<int> coin(0, 1);
    float rad = angle_dis(gen) * (std::numbers::pi_v<float> / 180.f);

    Ball ball {(float)SPEED * std::cos(rad) * (coin(gen) ? 1.f : -1.f),
               (float)SPEED * std::sin(rad) * (coin(gen) ? 1.f : -1.f)};
    Paddle paddle {};
    std::bitset<256> *keyset = new std::bitset<256>();

    WNDCLASS wc {};
    const wchar_t *CLASS_NAME = L"Pong";
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
    RECT window = {0, 0, WIDTH, HEIGHT + 40};
    DWORD WindowStyles = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
    AdjustWindowRectEx(&window, WindowStyles, FALSE, 0);
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"GGPong", WindowStyles, CW_USEDEFAULT, CW_USEDEFAULT,
                                window.right - window.left, window.bottom - window.top, NULL, NULL, hInstance, keyset);
    ShowWindow(hwnd, nCmdShow);
    MSG msg {};
    RenderContext rc {};
    InitRenderContext(rc, hwnd);

    LARGE_INTEGER freq, prev, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    do {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        QueryPerformanceCounter(&now);
        float dt = static_cast<float>(now.QuadPart - prev.QuadPart) / freq.QuadPart;
        update(ball, paddle, dt, *keyset);
        render(ball, paddle, rc);
        ValidateRect(hwnd, NULL);
        prev = now;
    } while (true);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CREATESTRUCTW *lpPtr = reinterpret_cast<CREATESTRUCTW *>(lParam);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(lpPtr->lpCreateParams));
            return 0;
        }
        case WM_KEYDOWN: {
            std::bitset<256> *keyset = reinterpret_cast<std::bitset<256> *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            if (keyset != nullptr && wParam < 256) keyset->set(wParam);
            return 0;
        }
        case WM_KEYUP: {
            std::bitset<256> *keyset = reinterpret_cast<std::bitset<256> *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            if (keyset != nullptr && wParam < 256) keyset->reset(wParam);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            delete reinterpret_cast<std::bitset<256> *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}