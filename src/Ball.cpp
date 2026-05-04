#include "Ball.h"
#include "Constants.h"

Ball::Ball()
    : position{0, 0}, velocity{0, 0}, radius(Constants::BALL_RADIUS), launched(false)
{
    Reset();
}

void Ball::Update(float dt)
{
    if (!launched) return;

    if(IsKeyDown(KEY_SPACE)) {
        position.x += 2 * velocity.x * dt;
        position.y += 2 * velocity.y * dt;
    }
    else{
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }

    

    if (position.x - radius <= 0) {
        position.x = radius;
        velocity.x = -velocity.x;
    }
    if (position.x + radius >= Constants::SCREEN_WIDTH) {
        position.x = Constants::SCREEN_WIDTH - radius;
        velocity.x = -velocity.x;
    }
    if (position.y - radius <= 0) {
        position.y = radius;
        velocity.y = -velocity.y;
    }
}

void Ball::Draw() const
{
    DrawCircleV(position, radius, WHITE);
}

void Ball::Launch()
{
    if (!launched) {
        launched = true;
        velocity = {Constants::BALL_SPEED * 0.3f, -Constants::BALL_SPEED};
    }
}

void Ball::Reset()
{
    launched = false;
    position = {Constants::SCREEN_WIDTH / 2.0f, Constants::PADDLE1_Y - radius - 5};
    velocity = {0, 0};
}
