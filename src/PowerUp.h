#pragma once
#include "raylib.h"
#include "Projectile.h"

enum class PickupType { HEAVY, HOMING, SCATTER, HEAL };

class PowerUp {
public:
    PowerUp();

    void Spawn(Vector2 pos, PickupType t, float dir = 1.0f);
    void Update(float dt);
    void Draw() const;
    void Deactivate();

    Vector2     position;
    PickupType  type;
    bool        active;

private:
    float size;
    float fallDir;
};
