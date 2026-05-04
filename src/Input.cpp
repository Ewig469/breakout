#include "Input.h"
#include "raylib.h"

int Input::P1MoveDir() const
{
    int d = 0;
    if (IsKeyDown(KEY_D)) d++;
    if (IsKeyDown(KEY_A)) d--;
    return d;
}

bool Input::P1Action() const
{
    return IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
}

bool Input::P1Fire() const
{
    return IsKeyPressed(KEY_F);
}

int Input::P2MoveDir() const
{
    int d = 0;
    if (IsKeyDown(KEY_RIGHT)) d++;
    if (IsKeyDown(KEY_LEFT))  d--;
    return d;
}

bool Input::P2Action() const
{
    return IsKeyPressed(KEY_KP_1);
}

bool Input::P2Fire() const
{
    return IsKeyPressed(KEY_KP_2);
}

bool Input::UpPressed() const
{
    return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
}

bool Input::DownPressed() const
{
    return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
}
