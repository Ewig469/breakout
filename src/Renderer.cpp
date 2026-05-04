#include "Renderer.h"
#include "Constants.h"
#include "raylib.h"

void Renderer::Init()
{
    InitWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, "Breakout");
    SetTargetFPS(60);
}

void Renderer::BeginFrame()
{
    BeginDrawing();
    ClearBackground(BLACK);
}

void Renderer::EndFrame()
{
    EndDrawing();
}

void Renderer::Close()
{
    CloseWindow();
}

bool Renderer::ShouldClose() const
{
    return WindowShouldClose();
}
