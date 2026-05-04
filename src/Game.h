#pragma once
#include <vector>
#include "Renderer.h"
#include "Input.h"
#include "Ball.h"
#include "Paddle.h"
#include "BrickGrid.h"
#include "Enemy.h"
#include "Projectile.h"
#include "PowerUp.h"

struct PlayerState {
    Paddle paddle;
    Ball   ball;
    std::vector<ProjType> inventory;
    int hp;
    int maxHp;
    bool alive;
};

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    enum State { MODE_SELECT, MENU, PLAYING, GAME_OVER, VICTORY };

    void HandleModeSelect();
    void HandleMenu();
    void HandlePlaying(float dt);
    void HandleGameOver();
    void HandleVictory();

    void ApplyPlayerDamage(PlayerState& pl, int damage);
    void HandleBall(PlayerState& pl, bool isP2);
    void CheckPaddleCollision(Ball& b, const Paddle& pad);
    void CheckBallEnemyCollision(Ball& b);
    void SpawnPowerUp(Vector2 pos);
    void CollectPowerUp(PlayerState& pl, PowerUp& pu);
    void FirePlayerProjectile(ProjType type, Vector2 origin);
    void HandleScatter(Vector2 pos);
    void CleanupInactive();
    void ResetGame();
    void DrawPlayer(const PlayerState& pl, Color padColor) const;
    void DrawHUD() const;

    Renderer  renderer;
    Input     input;
    BrickGrid brickGrid;
    Enemy     enemy;

    PlayerState p1;
    PlayerState p2;

    std::vector<Projectile> enemyProjectiles;
    std::vector<Projectile> playerProjectiles;
    std::vector<PowerUp>    powerUps;

    Texture2D texMenu;
    Texture2D texGameOver;
    Texture2D texVictory;

    State state;
    int   score;
    int   bossSkillAccumulator;
    int   modeSelection;
    bool  twoPlayer;
};
