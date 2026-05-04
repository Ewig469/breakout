#pragma once
#include "raylib.h"

class Paddle {
public:
    Paddle(float yOffset);

    void Update(float dt, int moveDir);
    void Draw(Color bodyColor) const;
    void Reset();

    Rectangle rect;
    float speed;
    float baseY;
};
