#pragma once
#include "raylib.h"

class Enemy {
public:
    Enemy();

    void Update(float dt);
    void Draw() const;
    void TakeDamage(int amount);
    void Reset();

    bool ConsumeShoot();
    Vector2 GetShootPos() const;

    Rectangle rect;
    int hp;
    int maxHp;
    float speed;
    bool isDead;

private:
    float shootTimer;
    float shootCooldown;
    bool  wantsToShoot;
    float dirChangeTimer;
    float movePhase;
    float baseY;
    int   dirX;

    void ResetShootTimer();
};
