#include "pch.h"
#include "timer.h"

ULONGLONG Timer::currentTime = 0;
ULONGLONG Timer::previousFrameTime = 0;

void Timer::Start()
{
    currentTime = GetTickCount64();
    previousFrameTime = currentTime;
}

void Timer::Update()
{
    previousFrameTime = currentTime;
    currentTime = GetTickCount64();
}

float Timer::GetDeltaTime()
{
    return static_cast<float>(currentTime - previousFrameTime) / 1000.0f;
}
