#pragma once

struct Vector2
{
    float x;
    float y;
};

class Game
{
public:
    Game(int width, int height, const wchar_t* title);

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    void OnKeyDown(UINT8 key);
    void OnKeyUp(UINT8 key);

    bool GetKeyDown(UINT8 key);

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    const wchar_t* GetTitle() const noexcept { return m_title.c_str(); }

private:
    int m_width;
    int m_height;
    std::wstring m_title;

    std::bitset<256> m_keys;

    struct Renderer
    {
        HDC hdc;
        HDC memDC;
        HBITMAP backBuffer;
    } m_renderer;

    HPEN m_colorSideX;
    HPEN m_colorSideY;
    HPEN m_colorFloor;
    HBRUSH m_colorCeiling;

    struct Camera
    {
        Vector2 pos;
        Vector2 dir;
        Vector2 plane;
    } m_cam;

    static constexpr int kMaxMapDimension = 32;
    static constexpr int kMaxMapDataSize = kMaxMapDimension * kMaxMapDimension;

    struct Map
    {
        char& operator()(int column, int row)
        {
            return data[column + row * width];
        }

        int width;
        int height;
        std::array<char, kMaxMapDataSize> data;
    } m_map;
};
