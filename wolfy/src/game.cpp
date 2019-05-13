#include "pch.h"
#include "game.h"
#include "win32_app.h"

constexpr int sgn(float val)
{
    return (0 < val) - (val < 0);
}

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

    m_colorSideX = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
    m_colorSideY = CreatePen(PS_SOLID, 0, RGB(127, 0, 0));
    m_colorFloor = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
    m_colorCeiling = CreateSolidBrush(RGB(48, 48, 48));

    m_cam =
    {
        Vector2{ 8.0f, 8.0f },
        Vector2{ -1.0f, 0.0f },
        Vector2{ 0.0f, 0.66f },
    };

    m_map =
    {
        16,
        16,
        {
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1,
          1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1,
          1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
          1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        },
    };
}

void Game::OnUpdate()
{
    // Clear screen
    static RECT rc = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    FillRect(m_renderer.memDC, &rc, m_colorCeiling);

    // Generate and draw level
    for (int x = 0; x < m_width; x++)
    {
        float cameraX = 2.0f * static_cast<float>(x) / static_cast<float>(m_width - 1) - 1.0f;
        float rayDirX = m_cam.dir.x + m_cam.plane.x * cameraX;
        float rayDirY = m_cam.dir.y + m_cam.plane.y * cameraX;

        int mapX = static_cast<int>(m_cam.pos.x);
        int mapY = static_cast<int>(m_cam.pos.y);

        float deltaDistX = std::abs(1.0f / rayDirX);
        float deltaDistY = std::abs(1.0f / rayDirY);

        int stepX = sgn(rayDirX);
        int stepY = sgn(rayDirY);

        float sideDistX;
        float sideDistY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (m_cam.pos.x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0f - m_cam.pos.x) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (m_cam.pos.y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0f - m_cam.pos.y) * deltaDistY;
        }

        int hit = 0;
        int side;

        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (m_map(mapX, mapY) > 0) hit = 1;
        }

        float perpWallDist;

        if (side == 0)
        {
            perpWallDist = (mapX - m_cam.pos.x + (1 - stepX) / 2) / rayDirX;
        }
        else
        {
            perpWallDist = (mapY - m_cam.pos.y + (1 - stepY) / 2) / rayDirY;
        }

        int ceiling = static_cast<int>(static_cast<float>(m_height) / 2.0f - static_cast<float>(m_height) / perpWallDist);
        int floor = m_height - ceiling;

        // Draw wall
        if (side == 0)
        {
            SelectObject(m_renderer.memDC, m_colorSideX);
        }
        else
        {
            SelectObject(m_renderer.memDC, m_colorSideY);
        }

        MoveToEx(m_renderer.memDC, x, ceiling, nullptr);
        LineTo(m_renderer.memDC, x, floor);

        // Draw floor
        SelectObject(m_renderer.memDC, m_colorFloor);
        MoveToEx(m_renderer.memDC, x, floor, nullptr);
        LineTo(m_renderer.memDC, x, m_height);
    }
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

void Game::OnKeyDown(UINT8 key)
{
    m_keys.set(key, true);
}

void Game::OnKeyUp(UINT8 key)
{
    m_keys.set(key, false);
}

bool Game::GetKeyDown(UINT8 key)
{
    return m_keys.test(key);
}
