#include "Ball.h"
#include "Constants.h"

Ball::Ball()
    : position{0, 0}, velocity{0, 0}, radius(Constants::BALL_RADIUS),
      ballColor(WHITE), invertWalls(false),
      launched(false), speedBoostTimer(0), isSpeedBoosted(false)
{
    Reset();
}

void Ball::Update(float dt)
{
    if (!launched) return;

    if (isSpeedBoosted) {
        speedBoostTimer -= dt;
        if (speedBoostTimer <= 0)
            isSpeedBoosted = false;
    }

    float mult = isSpeedBoosted ? Constants::BALL_SPEED_BOOST_MULTIPLIER : 1.0f;
    float spd = IsKeyDown(KEY_SPACE) ? 2.0f : 1.0f;
    mult *= spd;

    position.x += velocity.x * mult * dt;
    position.y += velocity.y * mult * dt;

    // Side walls
    if (position.x - radius <= 0) {
        position.x = radius;
        velocity.x = -velocity.x;
    }
    if (position.x + radius >= Constants::SCREEN_WIDTH) {
        position.x = Constants::SCREEN_WIDTH - radius;
        velocity.x = -velocity.x;
    }

    // Vertical walls — depending on orientation
    if (invertWalls) {
        // P2: bounce at bottom, no bounce at top (death zone)
        if (position.y + radius >= Constants::SCREEN_HEIGHT) {
            position.y = Constants::SCREEN_HEIGHT - radius;
            velocity.y = -velocity.y;
        }
    } else {
        // P1: bounce at top, no bounce at bottom (death zone)
        if (position.y - radius <= 0) {
            position.y = radius;
            velocity.y = -velocity.y;
        }
    }
}

void Ball::Draw() const
{
    Color c = isSpeedBoosted ? ORANGE : ballColor;
    DrawCircleV(position, radius, c);
    DrawCircleLines(position.x, position.y, radius + 1, {255,255,255,100});
    if (isSpeedBoosted)
        DrawCircleLines(position.x, position.y, radius + 2, YELLOW);
}

void Ball::Launch(float yDir)
{
    if (!launched) {
        launched = true;
        velocity = {Constants::BALL_SPEED * 0.3f, yDir * Constants::BALL_SPEED};
    }
}

void Ball::Reset()
{
    launched = false;
    isSpeedBoosted = false;
    speedBoostTimer = 0;
    position = {Constants::SCREEN_WIDTH / 2.0f,
                invertWalls ? Constants::PADDLE_VERSUS_Y + Constants::PADDLE_HEIGHT + radius + 5
                            : Constants::PADDLE1_Y - radius - 5};
    velocity = {0, 0};
}

void Ball::ApplySpeedBoost()
{
    isSpeedBoosted = true;
    speedBoostTimer = Constants::BALL_SPEED_BOOST_DURATION;
}
