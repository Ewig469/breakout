#pragma once
#include "raylib.h"

enum class ProjType { ENEMY, HEAVY, HOMING, SCATTER, SCATTER_FRAG };

class Projectile {
public:
    Projectile();

    void Fire(Vector2 pos, Vector2 vel, ProjType t, int owner = 0);
    void Update(float dt);
    void UpdateHoming(float dt, Vector2 target);
    void Draw() const;
    void Deactivate();

    Vector2 position;
    Vector2 velocity;
    float   size;
    ProjType type;
    int     damage;
    bool    active;
    int     ownerId;  // 0=none, 1=P1, 2=P2, 3=enemy
};
