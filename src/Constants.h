#pragma once

namespace Constants {

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr float PADDLE_WIDTH = 120.0f;
constexpr float PADDLE_HEIGHT = 20.0f;
constexpr float PADDLE_SPEED = 500.0f;
constexpr float PADDLE1_Y = SCREEN_HEIGHT - 50.0f;
constexpr float PADDLE2_Y = SCREEN_HEIGHT - 85.0f;
constexpr float PADDLE_VERSUS_Y = 20.0f;

constexpr float BALL_RADIUS = 8.0f;
constexpr float BALL_SPEED = 400.0f;
constexpr float BALL_MAX_BOUNCE_ANGLE = 1.0472f;
constexpr float BALL_SPEED_BOOST_MULTIPLIER = 1.8f;
constexpr float BALL_SPEED_BOOST_DURATION = 3.0f;

constexpr int BRICK_ROWS = 6;
constexpr int BRICK_COLS = 10;
constexpr float BRICK_WIDTH = 70.0f;
constexpr float BRICK_HEIGHT = 25.0f;
constexpr float BRICK_PADDING = 4.0f;
constexpr float BRICK_OFFSET_TOP = 80.0f;
constexpr float BRICK_OFFSET_LEFT = 45.0f;
constexpr float SPEED_BOOST_BRICK_CHANCE = 0.08f;

constexpr int PLAYER_HP = 100;
constexpr int VERSUS_HP = 200;
constexpr int ENEMY_HP = 300;
constexpr int HEAL_AMOUNT = 20;

constexpr float ENEMY_WIDTH = 80.0f;
constexpr float ENEMY_HEIGHT = 30.0f;
constexpr float ENEMY_SPEED = 150.0f;
constexpr float ENEMY_Y = 15.0f;
constexpr float ENEMY_SHOOT_MIN = 1.2f;
constexpr float ENEMY_SHOOT_MAX = 2.5f;

constexpr float PROJECTILE_SIZE = 8.0f;
constexpr float ENEMY_PROJECTILE_SPEED = 300.0f;
constexpr float HEAVY_PROJECTILE_SPEED = 500.0f;
constexpr float HOMING_PROJECTILE_SPEED = 350.0f;
constexpr float SCATTER_PROJECTILE_SPEED = 400.0f;
constexpr float SCATTER_FRAG_SPEED = 220.0f;

constexpr int ENEMY_PROJECTILE_DAMAGE = 20;
constexpr int HEAVY_DAMAGE = 50;
constexpr int HOMING_DAMAGE = 20;
constexpr int SCATTER_DAMAGE = 30;
constexpr int SCATTER_FRAG_DAMAGE = 15;
constexpr int BALL_FALL_DAMAGE = 10;
constexpr int BALL_ENEMY_DAMAGE = 15;

constexpr float POWERUP_SIZE = 14.0f;
constexpr float POWERUP_FALL_SPEED = 150.0f;
constexpr float POWERUP_DROP_CHANCE = 0.35f;

constexpr float HOMING_TURN_RATE = 6.0f;

constexpr int BOSS_SKILL_HP_THRESHOLD = 20;
constexpr int BOSS_SKILL_BRICK_COUNT = 5;

} // namespace Constants
