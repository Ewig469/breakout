#include "Brick.h"

Brick::Brick(float x, float y, float width, float height, int hp)
    : rect{x, y, width, height}, hp(hp), destroyed(false), initialHp(hp)
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
    switch (hp) {
    case 3: color = RED;    break;
    case 2: color = ORANGE; break;
    default: color = YELLOW; break;
    }
    DrawRectangleRec(rect, color);
    DrawRectangleLinesEx(rect, 1, DARKGRAY);
}
