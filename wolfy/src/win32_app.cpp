#include "pch.h"
#include "win32_app.h"
#include "game.h"

HWND Win32Application::m_hwnd = nullptr;

int Win32Application::Run(Game* pGame, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"WolfyClass";
    RegisterClassEx(&wcex);

    RECT rc = { 0, 0, static_cast<LONG>(pGame->GetWidth()), static_cast<LONG>(pGame->GetHeight()) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(
        wcex.lpszClassName,
        pGame->GetTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        hInstance,
        pGame);

    pGame->OnInit();

    ShowWindow(m_hwnd, nCmdShow);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    pGame->OnDestroy();

    return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK Win32Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto pGame = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;

    case WM_PAINT:
        {
            if (pGame)
            {
                pGame->OnUpdate();
                pGame->OnRender();
            }
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
