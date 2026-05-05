#pragma once
#include "raylib.h"

class Brick {
public:
    Brick(float x, float y, float width, float height, int hp, bool speedBoost = false);

    int Hit();
    void Draw() const;

    static void LoadTextures();
    static void UnloadTextures();

    Rectangle rect;
    int hp;
    bool destroyed;
    bool isSpeedBoost;

private:
    int initialHp;
};
