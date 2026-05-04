#include "PowerUp.h"
#include "Constants.h"

PowerUp::PowerUp()
    : position{0, 0}, type(PickupType::HEAVY), active(false),
      size(Constants::POWERUP_SIZE), fallDir(1.0f)
{
}

void PowerUp::Spawn(Vector2 pos, PickupType t, float dir)
{
    position = pos;
    type     = t;
    active   = true;
    fallDir  = dir;
}

void PowerUp::Update(float dt)
{
    if (!active) return;
    position.y += Constants::POWERUP_FALL_SPEED * fallDir * dt;
    if (position.y > Constants::SCREEN_HEIGHT + 20)
        active = false;
}

void PowerUp::Draw() const
{
    if (!active) return;

    Color c;
    const char* label = "";
    switch (type) {
    case PickupType::HEAVY:
        c = GOLD;   label = "H"; break;
    case PickupType::HOMING:
        c = GREEN;  label = "T"; break;
    case PickupType::SCATTER:
        c = PURPLE; label = "S"; break;
    case PickupType::HEAL:
        c = SKYBLUE; label = "+"; break;
    }

    DrawRectangle(position.x - size/2, position.y - size/2, size, size, c);
    DrawRectangleLines(position.x - size/2, position.y - size/2, size, size, DARKGRAY);
    DrawText(label, position.x - 4, position.y - 5, 10, BLACK);
}

void PowerUp::Deactivate()
{
    active = false;
}
