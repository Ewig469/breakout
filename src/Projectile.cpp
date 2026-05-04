#include "Projectile.h"
#include "Constants.h"
#include <cmath>

Projectile::Projectile()
    : position{0, 0}, velocity{0, 0}, size(Constants::PROJECTILE_SIZE),
      type(ProjType::ENEMY), damage(0), active(false)
{
}

void Projectile::Fire(Vector2 pos, Vector2 vel, ProjType t)
{
    position = pos;
    velocity = vel;
    type     = t;
    active   = true;
    size     = Constants::PROJECTILE_SIZE;

    switch (t) {
    case ProjType::ENEMY:  damage = Constants::ENEMY_PROJECTILE_DAMAGE; break;
    case ProjType::HEAVY:  damage = Constants::HEAVY_DAMAGE;  size = 12.0f; break;
    case ProjType::HOMING: damage = Constants::HOMING_DAMAGE; size = 7.0f;  break;
    case ProjType::SCATTER: damage = Constants::SCATTER_DAMAGE; size = 9.0f; break;
    case ProjType::SCATTER_FRAG: damage = Constants::SCATTER_FRAG_DAMAGE; size = 7.0f; break;
    }
}

void Projectile::Update(float dt)
{
    if (!active) return;
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;

    if (position.y < -20 || position.y > Constants::SCREEN_HEIGHT + 20 ||
        position.x < -20 || position.x > Constants::SCREEN_WIDTH + 20)
        active = false;
}

void Projectile::UpdateHoming(float dt, Vector2 target)
{
    if (!active || type != ProjType::HOMING) return;

    Vector2 dir = {target.x - position.x, target.y - position.y};
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0) {
        dir.x /= len;
        dir.y /= len;
        float speed = Constants::HOMING_PROJECTILE_SPEED;
        velocity.x += (dir.x * speed - velocity.x) * Constants::HOMING_TURN_RATE * dt;
        velocity.y += (dir.y * speed - velocity.y) * Constants::HOMING_TURN_RATE * dt;
    }
}

void Projectile::Draw() const
{
    if (!active) return;
    Color c;
    switch (type) {
    case ProjType::ENEMY:        c = RED;     break;
    case ProjType::HEAVY:        c = GOLD;    break;
    case ProjType::HOMING:       c = GREEN;   break;
    case ProjType::SCATTER:      c = PURPLE;  break;
    case ProjType::SCATTER_FRAG: c = VIOLET;  break;
    }
    DrawCircleV(position, size, c);
    DrawCircleLines(position.x, position.y, size, DARKGRAY);
}

void Projectile::Deactivate()
{
    active = false;
}
