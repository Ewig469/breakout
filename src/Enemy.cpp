#include "Enemy.h"
#include "Constants.h"
#include <cstdlib>
#include <cmath>

Enemy::Enemy()
    : hp(Constants::ENEMY_HP), maxHp(Constants::ENEMY_HP),
      speed(Constants::ENEMY_SPEED), isDead(false),
      shootTimer(0), wantsToShoot(false),
      dirChangeTimer(0), movePhase(0), baseY(Constants::ENEMY_Y), dirX(1)
{
    Reset();
}

void Enemy::Update(float dt)
{
    if (isDead) return;

    // Direction change timer
    dirChangeTimer -= dt;
    if (dirChangeTimer <= 0) {
        int r = rand() % 10;
        if (r < 3) {
            dirX = (rand() % 2) ? 1 : -1;
        } else if (r < 6) {
            speed = Constants::ENEMY_SPEED * (0.6f + (float)rand() / RAND_MAX * 1.2f);
        }
        dirChangeTimer = 1.0f + (float)rand() / RAND_MAX * 2.5f;
    }

    rect.x += speed * dirX * dt;
    if (rect.x <= 0) { rect.x = 0; dirX = 1; }
    if (rect.x + rect.width >= Constants::SCREEN_WIDTH) {
        rect.x = Constants::SCREEN_WIDTH - rect.width;
        dirX = -1;
    }

    movePhase += dt * 2.5f;
    rect.y = baseY + std::sin(movePhase) * 8.0f;

    shootTimer -= dt;
    if (shootTimer <= 0) {
        wantsToShoot = true;
        ResetShootTimer();
    }
}

void Enemy::Draw() const
{
    if (isDead) return;

    DrawRectangleRec(rect, MAROON);
    DrawRectangleLinesEx(rect, 2, DARKGRAY);

    float eyeR = 4.0f;
    float eyeY = rect.y + rect.height / 2.0f;
    DrawCircle(rect.x + rect.width * 0.3f, eyeY, eyeR, RED);
    DrawCircle(rect.x + rect.width * 0.7f, eyeY, eyeR, RED);

    float barW = rect.width;
    float barH = 6.0f;
    float barX = rect.x;
    float barY = rect.y - barH - 8;
    float ratio = (float)hp / maxHp;

    DrawRectangle(barX, barY, barW, barH, DARKGRAY);
    DrawRectangle(barX, barY, barW * ratio, barH, RED);
    DrawRectangleLines(barX, barY, barW, barH, LIGHTGRAY);
}

void Enemy::TakeDamage(int amount)
{
    if (isDead) return;
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
        isDead = true;
    }
}

void Enemy::Reset()
{
    rect = {
        Constants::SCREEN_WIDTH / 2.0f - Constants::ENEMY_WIDTH / 2.0f,
        Constants::ENEMY_Y,
        Constants::ENEMY_WIDTH,
        Constants::ENEMY_HEIGHT
    };
    baseY = Constants::ENEMY_Y;
    hp = maxHp;
    isDead = false;
    dirX = 1;
    speed = Constants::ENEMY_SPEED;
    movePhase = 0;
    wantsToShoot = false;
    ResetShootTimer();
    dirChangeTimer = 1.5f + (float)rand() / RAND_MAX * 1.0f;
}

bool Enemy::ConsumeShoot()
{
    if (isDead) return false;
    bool result = wantsToShoot;
    wantsToShoot = false;
    return result;
}

Vector2 Enemy::GetShootPos() const
{
    return {rect.x + rect.width / 2.0f, rect.y + rect.height + 5};
}

void Enemy::ResetShootTimer()
{
    float range = Constants::ENEMY_SHOOT_MAX - Constants::ENEMY_SHOOT_MIN;
    shootCooldown = Constants::ENEMY_SHOOT_MIN + (float)rand() / RAND_MAX * range;
    shootTimer = shootCooldown;
}
