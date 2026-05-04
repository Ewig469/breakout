#include "Paddle.h"
#include "Constants.h"

Paddle::Paddle(float yOffset)
    : speed(Constants::PADDLE_SPEED), baseY(yOffset)
{
    Reset();
}

void Paddle::Update(float dt, int moveDir)
{
    rect.x += moveDir * speed * dt;

    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.width > Constants::SCREEN_WIDTH)
        rect.x = Constants::SCREEN_WIDTH - rect.width;
}

void Paddle::Draw(Color bodyColor) const
{
    DrawRectangleRec(rect, bodyColor);
    DrawRectangleLinesEx(rect, 2, DARKGRAY);
}

void Paddle::Reset()
{
    rect = {
        Constants::SCREEN_WIDTH / 2.0f - Constants::PADDLE_WIDTH / 2.0f,
        baseY,
        Constants::PADDLE_WIDTH,
        Constants::PADDLE_HEIGHT
    };
}
