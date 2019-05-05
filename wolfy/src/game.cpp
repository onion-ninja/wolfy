#include "pch.h"
#include "game.h"
#include "win32_app.h"

Game::Game(int width, int height, const wchar_t* title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
{
}

void Game::OnInit()
{
    HDC hdc = GetDC(Win32Application::GetHwnd());
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP backBuffer = CreateCompatibleBitmap(hdc, m_width, m_height);
    SelectObject(memDC, backBuffer);

    m_renderer = { hdc, memDC, backBuffer };
}

void Game::OnUpdate()
{
}

void Game::OnRender()
{
    // Present rendered image
    BitBlt(m_renderer.hdc, 0, 0, m_width, m_height, m_renderer.memDC, 0, 0, SRCCOPY);
}

void Game::OnDestroy()
{
    DeleteObject(m_renderer.backBuffer);
    DeleteDC(m_renderer.memDC);
    ReleaseDC(Win32Application::GetHwnd(), m_renderer.hdc);
}
