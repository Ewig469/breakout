#include "Brick.h"

Brick::Brick(float x, float y, float width, float height, int hp, bool speedBoost)
    : rect{x, y, width, height}, hp(hp), destroyed(false), isSpeedBoost(speedBoost),
      initialHp(hp)
{
}

int Brick::Hit()
{
    hp--;
    if (hp <= 0) {
        destroyed = true;
        return initialHp * 10;
    }
    return 0;
}

void Brick::Draw() const
{
    if (destroyed) return;

    Color color;
    if (isSpeedBoost)
        color = {0, 255, 255, 255};  // CYAN
    else switch (hp) {
    case 3: color = RED;    break;
    case 2: color = ORANGE; break;
    default: color = YELLOW; break;
    }
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 1, DARKGRAY);

    if (isSpeedBoost) {
        DrawText(">>", rect.x + rect.width/2 - 10, rect.y + 5, 10, BLACK);
    }
}
