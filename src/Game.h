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

enum class GameMode { SINGLE, COOP, VERSUS };

struct PlayerState {
    Paddle paddle;
    Ball   ball;
    std::vector<ProjType> inventory;
    int hp;
    int maxHp;
    int score;
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
    void HandlePlayingCoop(float dt);
    void HandlePlayingVersus(float dt);
    void HandleGameOver();
    void HandleVictory();

    void ApplyPlayerDamage(PlayerState& pl, int damage);
    void HandleBall(PlayerState& pl, bool isP2);
    void HandleBallVersus(PlayerState& pl, bool isP2);
    bool CheckPaddleCollision(Ball& b, const Paddle& pad);
    void CheckBallEnemyCollision(Ball& b);
    void SpawnPowerUp(Vector2 pos, float dir = 1.0f);
    void CollectPowerUp(PlayerState& pl, PowerUp& pu);
    void FirePlayerProjectile(ProjType type, Vector2 origin, int ownerId);
    void HandleScatter(Vector2 pos, int ownerId);
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
    std::vector<Projectile> p1Projectiles;
    std::vector<Projectile> p2Projectiles;
    std::vector<PowerUp>    powerUps;

    Texture2D texMenu;
    Texture2D texGameOver;
    Texture2D texVictory;

    GameMode gameMode;
    State    state;
    int      bossSkillAccumulator;
    int      modeSelection;
};
