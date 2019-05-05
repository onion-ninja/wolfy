#pragma once

class Game;

class Win32Application
{
public:
    static int Run(Game* pGame, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() noexcept { return m_hwnd; }

protected:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static HWND m_hwnd;
};
