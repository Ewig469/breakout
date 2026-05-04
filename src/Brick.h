#pragma once
#include "raylib.h"

class Brick {
public:
    Brick(float x, float y, float width, float height, int hp);

    int Hit();
    void Draw() const;

    Rectangle rect;
    int hp;
    bool destroyed;

private:
    int initialHp;
};
