#pragma once

class Timer
{
public:
    static void Start();
    static void Update();
    static float GetDeltaTime();

private:
    static ULONGLONG currentTime;
    static ULONGLONG previousFrameTime;
};
