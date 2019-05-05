#include "pch.h"
#include "game.h"
#include "win32_app.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    Game game(800, 600, L"Wolfy");
    return Win32Application::Run(&game, hInstance, nCmdShow);
}
