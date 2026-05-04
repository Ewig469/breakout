#pragma once
#include "raylib.h"

class Ball {
public:
    Ball();

    void Update(float dt);
    void Draw() const;
    void Launch();
    void Reset();
    bool IsLaunched() const { return launched; }

    Vector2 position;
    Vector2 velocity;
    float radius;

private:
    bool launched;
};
