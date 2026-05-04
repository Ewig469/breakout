#pragma once
#include "raylib.h"

class Ball {
public:
    Ball();

    void Update(float dt);
    void Draw() const;
    void Launch(float yDir);
    void Launch() { Launch(-1.0f); }
    void Reset();
    bool IsLaunched() const { return launched; }
    void ApplySpeedBoost();

    Vector2 position;
    Vector2 velocity;
    float radius;
    Color ballColor;
    bool invertWalls;  // true = bounce at bottom, die at top (P2)

private:
    bool launched;
    float speedBoostTimer;
    bool isSpeedBoosted;
};
