#pragma once

class Game
{
public:
    Game(int width, int height, const wchar_t* title);

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    const wchar_t* GetTitle() const noexcept { return m_title.c_str(); }

private:
    int m_width;
    int m_height;
    std::wstring m_title;

    struct Renderer
    {
        HDC hdc;
        HDC memDC;
        HBITMAP backBuffer;
    } m_renderer;
};
