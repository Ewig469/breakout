#include "Game.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
    : state(MODE_SELECT), score(0), bossSkillAccumulator(0),
      modeSelection(0), twoPlayer(false),
      p1{Paddle(Constants::PADDLE1_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, true},
      p2{Paddle(Constants::PADDLE2_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, true}
{
    std::srand((unsigned)std::time(nullptr));
}

Game::~Game()
{
    UnloadTexture(texMenu);
    UnloadTexture(texGameOver);
    UnloadTexture(texVictory);
}

void Game::Run()
{
    renderer.Init();

    // Load textures after OpenGL context is ready, using absolute path
    const char* appDir = GetApplicationDirectory();
    texMenu     = LoadTexture(TextFormat("%s../picture/界面图.png", appDir));
    texGameOver = LoadTexture(TextFormat("%s../picture/战败图.png", appDir));
    texVictory  = LoadTexture(TextFormat("%s../picture/战胜图.png", appDir));

    while (!renderer.ShouldClose()) {
        float dt = GetFrameTime();
        renderer.BeginFrame();

        switch (state) {
        case MODE_SELECT: HandleModeSelect();    break;
        case MENU:        HandleMenu();          break;
        case PLAYING:     HandlePlaying(dt);     break;
        case GAME_OVER:   HandleGameOver();      break;
        case VICTORY:     HandleVictory();       break;
        }

        renderer.EndFrame();
    }

    renderer.Close();
}

// ===== MODE SELECT =====

void Game::HandleModeSelect()
{
    DrawTexturePro(texMenu, {0,0,(float)texMenu.width,(float)texMenu.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,90});

    DrawText("BREAKOUT", Constants::SCREEN_WIDTH / 2 - 100, 150, 50, WHITE);
    DrawText("Select Mode", Constants::SCREEN_WIDTH / 2 - 80, 220, 30, LIGHTGRAY);

    Color c1 = (modeSelection == 0) ? YELLOW : GRAY;
    Color c2 = (modeSelection == 1) ? YELLOW : GRAY;
    const char* m1 = (modeSelection == 0) ? "> 1 Player" : "  1 Player";
    const char* m2 = (modeSelection == 1) ? "> 2 Players" : "  2 Players";

    DrawText(m1, Constants::SCREEN_WIDTH / 2 - 70, 280, 26, c1);
    DrawText(m2, Constants::SCREEN_WIDTH / 2 - 70, 320, 26, c2);

    DrawText("UP / DOWN to choose  |  ENTER / SPACE to confirm",
             Constants::SCREEN_WIDTH / 2 - 190, 400, 16, DARKGRAY);

    if (input.UpPressed() || input.DownPressed())
        modeSelection = 1 - modeSelection;

    if (input.P1Action()) {
        twoPlayer = (modeSelection == 1);
        state = MENU;
    }
}

// ===== MENU =====

void Game::HandleMenu()
{
    DrawTexturePro(texMenu, {0,0,(float)texMenu.width,(float)texMenu.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,90});

    DrawText(twoPlayer ? "BREAKOUT - 2 Players" : "BREAKOUT - 1 Player",
             Constants::SCREEN_WIDTH / 2 - (twoPlayer ? 180 : 120), 130, 40, WHITE);
    DrawText("Press SPACE to start", Constants::SCREEN_WIDTH / 2 - 120, 200, 22, LIGHTGRAY);

    DrawText("P1 :  A / D move  |  SPACE launch  |  F fire",
             60, 270, 16, GRAY);
    if (twoPlayer) {
        DrawText("P2 :  <- -> move  |  KP_1  launch  |  KP_2 fire",
                 60, 300, 16, GRAY);
    }
    DrawText("Pickups: H=heavy(50)  T=homing(20)  S=scatter(30)  +=heal(20)",
             80, 350, 14, DARKGRAY);

    if (input.P1Action()) {
        ResetGame();
        state = PLAYING;
    }
}

// ===== GAME OVER / VICTORY =====

void Game::HandleGameOver()
{
    DrawTexturePro(texGameOver, {0,0,(float)texGameOver.width,(float)texGameOver.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,80});

    DrawText("GAME OVER", Constants::SCREEN_WIDTH / 2 - 150, 220, 40, RED);
    DrawText(TextFormat("Final Score: %d", score),
             Constants::SCREEN_WIDTH / 2 - 80, 280, 20, WHITE);
    DrawText("Press SPACE to restart", Constants::SCREEN_WIDTH / 2 - 110, 330, 20, LIGHTGRAY);

    if (input.P1Action()) { ResetGame(); state = PLAYING; }
}

void Game::HandleVictory()
{
    DrawTexturePro(texVictory, {0,0,(float)texVictory.width,(float)texVictory.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,80});

    DrawText("VICTORY!", Constants::SCREEN_WIDTH / 2 - 120, 220, 40, GREEN);
    DrawText(TextFormat("Final Score: %d", score),
             Constants::SCREEN_WIDTH / 2 - 80, 280, 20, WHITE);
    DrawText("Press SPACE to play again", Constants::SCREEN_WIDTH / 2 - 130, 330, 20, LIGHTGRAY);

    if (input.P1Action()) { ResetGame(); state = PLAYING; }
}

// ===== MAIN GAME LOOP =====

void Game::HandlePlaying(float dt)
{
    // ----- Player paddles -----
    if (p1.alive) p1.paddle.Update(dt, input.P1MoveDir());
    if (p2.alive && twoPlayer) p2.paddle.Update(dt, input.P2MoveDir());

    // ----- P1 Ball -----
    if (p1.alive) HandleBall(p1, false);

    // ----- P2 Ball -----
    if (p2.alive && twoPlayer) HandleBall(p2, true);

    // Check game-over / victory
    if (state != PLAYING) return;
    if (!p1.alive && (!twoPlayer || !p2.alive)) { state = GAME_OVER; return; }
    if (brickGrid.AllCleared() && enemy.isDead) { state = VICTORY; return; }

    // ----- Enemy AI -----
    enemy.Update(dt);
    if (enemy.ConsumeShoot()) {
        float targetX = Constants::SCREEN_WIDTH / 2.0f;
        int cnt = (p1.alive ? 1 : 0) + (p2.alive && twoPlayer ? 1 : 0);
        if (cnt > 0) {
            float sumX = 0;
            if (p1.alive) sumX += p1.paddle.rect.x + p1.paddle.rect.width / 2.0f;
            if (p2.alive && twoPlayer) sumX += p2.paddle.rect.x + p2.paddle.rect.width / 2.0f;
            targetX = sumX / cnt;
        }
        float dx = targetX - (enemy.rect.x + enemy.rect.width / 2.0f);
        float len = std::sqrt(dx * dx + Constants::SCREEN_HEIGHT * Constants::SCREEN_HEIGHT);
        if (len > 0) { dx /= len; }

        Projectile p;
        p.Fire(enemy.GetShootPos(),
               {dx * Constants::ENEMY_PROJECTILE_SPEED * 0.4f,
                Constants::ENEMY_PROJECTILE_SPEED},
               ProjType::ENEMY);
        enemyProjectiles.push_back(p);
    }

    // ----- Update projectiles -----
    for (auto& p : enemyProjectiles) p.Update(dt);
    Vector2 enemyCenter = {enemy.rect.x + enemy.rect.width / 2.0f,
                           enemy.rect.y + enemy.rect.height / 2.0f};
    for (auto& p : playerProjectiles) {
        if (p.type == ProjType::HOMING && !enemy.isDead)
            p.UpdateHoming(dt, enemyCenter);
        p.Update(dt);
    }

    // ----- Update power-ups -----
    for (auto& pu : powerUps) pu.Update(dt);

    // ----- Enemy projectiles hit players -----
    for (auto& p : enemyProjectiles) {
        if (!p.active) continue;
        if (p1.alive && CheckCollisionCircleRec(p.position, p.size, p1.paddle.rect)) {
            ApplyPlayerDamage(p1, p.damage);
            p.Deactivate();
            continue;
        }
        if (p2.alive && twoPlayer && CheckCollisionCircleRec(p.position, p.size, p2.paddle.rect)) {
            ApplyPlayerDamage(p2, p.damage);
            p.Deactivate();
        }
    }
    if (!p1.alive && (!twoPlayer || !p2.alive)) { state = GAME_OVER; return; }

    // ----- Player projectiles hit enemy -----
    if (!enemy.isDead) {
        for (auto& p : playerProjectiles) {
            if (!p.active) continue;
            if (p.type == ProjType::SCATTER_FRAG) continue;
            if (CheckCollisionCircleRec(p.position, p.size, enemy.rect)) {
                enemy.TakeDamage(p.damage);
                if (p.type == ProjType::SCATTER)
                    HandleScatter(p.position);
                p.Deactivate();
                if (enemy.isDead && brickGrid.AllCleared()) { state = VICTORY; return; }
            }
        }
    }

    // ----- Scatter fragments hit bricks -----
    for (auto& p : playerProjectiles) {
        if (!p.active || p.type != ProjType::SCATTER_FRAG) continue;
        int pts = brickGrid.CheckCircleCollision(p.position, p.size);
        if (pts > 0) {
            score += pts;
            p.Deactivate();
            auto drops = brickGrid.DrainDropPositions();
            for (auto& pos : drops) SpawnPowerUp(pos);
        }
    }

    // ----- Power-ups collected -----
    for (auto& pu : powerUps) {
        if (!pu.active) continue;
        Rectangle puRect = {pu.position.x - 8, pu.position.y - 8, 16, 16};
        if (p1.alive && CheckCollisionRecs(puRect, p1.paddle.rect)) { CollectPowerUp(p1, pu); continue; }
        if (p2.alive && twoPlayer && CheckCollisionRecs(puRect, p2.paddle.rect)) { CollectPowerUp(p2, pu); }
    }

    // ----- Player fire -----
    if (p1.alive && input.P1Fire() && !p1.inventory.empty()) {
        ProjType t = p1.inventory.front(); p1.inventory.erase(p1.inventory.begin());
        FirePlayerProjectile(t, {p1.paddle.rect.x + p1.paddle.rect.width / 2.0f, p1.paddle.rect.y});
    }
    if (p2.alive && twoPlayer && input.P2Fire() && !p2.inventory.empty()) {
        ProjType t = p2.inventory.front(); p2.inventory.erase(p2.inventory.begin());
        FirePlayerProjectile(t, {p2.paddle.rect.x + p2.paddle.rect.width / 2.0f, p2.paddle.rect.y});
    }

    // ----- Cleanup -----
    CleanupInactive();

    // ----- Draw -----
    brickGrid.Draw();
    enemy.Draw();
    if (p1.alive) DrawPlayer(p1, BLUE);
    if (p2.alive && twoPlayer) DrawPlayer(p2, SKYBLUE);
    for (const auto& p : enemyProjectiles) p.Draw();
    for (const auto& p : playerProjectiles) p.Draw();
    for (const auto& pu : powerUps) pu.Draw();
    DrawHUD();
}

// ===== BALL HANDLER (shared for both players) =====

void Game::HandleBall(PlayerState& pl, bool isP2)
{
    if (!pl.ball.IsLaunched()) {
        pl.ball.position.x = pl.paddle.rect.x + pl.paddle.rect.width / 2;
        pl.ball.position.y = pl.paddle.rect.y - pl.ball.radius - 1;

        bool launch = isP2 ? input.P2Action() : input.P1Action();
        if (launch) pl.ball.Launch();
        return;
    }

    pl.ball.Update(GetFrameTime());

    // Own paddle always
    CheckPaddleCollision(pl.ball, pl.paddle);

    // In 2P mode, other player's paddle can also hit this ball
    if (twoPlayer) {
        PlayerState& other = isP2 ? p1 : p2;
        if (other.alive)
            CheckPaddleCollision(pl.ball, other.paddle);
    }

    CheckBallEnemyCollision(pl.ball);
    score += brickGrid.CheckCollision(pl.ball);

    auto drops = brickGrid.DrainDropPositions();
    for (auto& pos : drops) SpawnPowerUp(pos);

    if (pl.ball.position.y > Constants::SCREEN_HEIGHT) {
        ApplyPlayerDamage(pl, Constants::BALL_FALL_DAMAGE);
        if (pl.alive) pl.ball.Reset();
    }
}

// ===== DAMAGE (with boss skill trigger) =====

void Game::ApplyPlayerDamage(PlayerState& pl, int damage)
{
    pl.hp -= damage;
    bossSkillAccumulator += damage;

    while (bossSkillAccumulator >= Constants::BOSS_SKILL_HP_THRESHOLD) {
        bossSkillAccumulator -= Constants::BOSS_SKILL_HP_THRESHOLD;
        if (!enemy.isDead)
            brickGrid.ReviveRandomBricks(Constants::BOSS_SKILL_BRICK_COUNT);
    }

    if (pl.hp <= 0) {
        pl.hp = 0;
        pl.alive = false;
    }
}

// ===== COLLISION HELPERS =====

void Game::CheckPaddleCollision(Ball& b, const Paddle& pad)
{
    if (b.velocity.y < 0) return;

    if (CheckCollisionCircleRec(b.position, b.radius, pad.rect)) {
        float hitPos = (b.position.x - pad.rect.x) / pad.rect.width;
        float angle  = (hitPos - 0.5f) * PI;

        float maxA = Constants::BALL_MAX_BOUNCE_ANGLE;
        if (angle >  maxA) angle =  maxA;
        if (angle < -maxA) angle = -maxA;

        float spd = std::sqrt(b.velocity.x * b.velocity.x +
                              b.velocity.y * b.velocity.y);
        b.velocity.x = spd * std::sin(angle);
        b.velocity.y = -spd * std::cos(angle);

        if (b.velocity.y > 0) b.velocity.y = -b.velocity.y;
        b.position.y = pad.rect.y - b.radius;
    }
}

void Game::CheckBallEnemyCollision(Ball& b)
{
    if (enemy.isDead) return;
    if (!CheckCollisionCircleRec(b.position, b.radius, enemy.rect)) return;

    enemy.TakeDamage(Constants::BALL_ENEMY_DAMAGE);

    float hitPos = (b.position.x - enemy.rect.x) / enemy.rect.width;
    float angle  = (hitPos - 0.5f) * PI * 0.7f;
    float spd    = std::sqrt(b.velocity.x * b.velocity.x +
                             b.velocity.y * b.velocity.y);

    b.velocity.x = spd * std::sin(angle);
    b.velocity.y = spd * std::cos(angle);
    if (b.velocity.y < 80.0f) b.velocity.y = 120.0f;
    b.position.y = enemy.rect.y + enemy.rect.height + b.radius;
}

// ===== POWER-UP / FIRE / SCATTER =====

void Game::SpawnPowerUp(Vector2 pos)
{
    PowerUp pu;
    PickupType t;
    int r = std::rand() % 100;
    if (r < 30)       t = PickupType::HEAVY;
    else if (r < 55)  t = PickupType::HOMING;
    else if (r < 80)  t = PickupType::SCATTER;
    else              t = PickupType::HEAL;
    pu.Spawn(pos, t);
    powerUps.push_back(pu);
}

void Game::CollectPowerUp(PlayerState& pl, PowerUp& pu)
{
    switch (pu.type) {
    case PickupType::HEAL:
        pl.hp += Constants::HEAL_AMOUNT;
        if (pl.hp > pl.maxHp) pl.hp = pl.maxHp;
        break;
    case PickupType::HEAVY:  pl.inventory.push_back(ProjType::HEAVY);   break;
    case PickupType::HOMING: pl.inventory.push_back(ProjType::HOMING);  break;
    case PickupType::SCATTER:pl.inventory.push_back(ProjType::SCATTER); break;
    }
    pu.Deactivate();
}

void Game::FirePlayerProjectile(ProjType type, Vector2 origin)
{
    Projectile p;
    switch (type) {
    case ProjType::HEAVY:
        p.Fire(origin, {0, -Constants::HEAVY_PROJECTILE_SPEED}, ProjType::HEAVY);
        break;
    case ProjType::HOMING: {
        Vector2 ec = {enemy.rect.x + enemy.rect.width / 2.0f,
                      enemy.rect.y + enemy.rect.height / 2.0f};
        Vector2 dir = {ec.x - origin.x, ec.y - origin.y};
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) { dir.x /= len; dir.y /= len; }
        p.Fire(origin,
               {dir.x * Constants::HOMING_PROJECTILE_SPEED,
                dir.y * Constants::HOMING_PROJECTILE_SPEED},
               ProjType::HOMING);
        break;
    }
    case ProjType::SCATTER:
        p.Fire(origin, {0, -Constants::SCATTER_PROJECTILE_SPEED}, ProjType::SCATTER);
        break;
    default: return;
    }
    playerProjectiles.push_back(p);
}

void Game::HandleScatter(Vector2 pos)
{
    for (int i = 0; i < 3; ++i) {
        Projectile frag;
        float angle = -0.9f + 0.9f * i;
        Vector2 vel = {
            std::sin(angle) * Constants::SCATTER_FRAG_SPEED,
            std::cos(angle) * Constants::SCATTER_FRAG_SPEED
        };
        frag.Fire(pos, vel, ProjType::SCATTER_FRAG);
        playerProjectiles.push_back(frag);
    }
}

void Game::CleanupInactive()
{
    enemyProjectiles.erase(
        std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
                       [](const Projectile& p) { return !p.active; }),
        enemyProjectiles.end());
    playerProjectiles.erase(
        std::remove_if(playerProjectiles.begin(), playerProjectiles.end(),
                       [](const Projectile& p) { return !p.active; }),
        playerProjectiles.end());
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(),
                       [](const PowerUp& pu) { return !pu.active; }),
        powerUps.end());
}

void Game::ResetGame()
{
    score = 0;
    bossSkillAccumulator = 0;
    p1 = {Paddle(Constants::PADDLE1_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, true};
    p2 = {Paddle(Constants::PADDLE2_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, true};
    enemyProjectiles.clear();
    playerProjectiles.clear();
    powerUps.clear();
    brickGrid.Reset();
    enemy.Reset();
}

// ===== DRAW =====

void Game::DrawPlayer(const PlayerState& pl, Color padColor) const
{
    pl.paddle.Draw(padColor);
    pl.ball.Draw();

    float startX = pl.paddle.rect.x + pl.paddle.rect.width + 6;
    float startY = pl.paddle.rect.y + 2;
    for (size_t i = 0; i < pl.inventory.size() && i < 5; ++i) {
        Color c; const char* l;
        switch (pl.inventory[i]) {
        case ProjType::HEAVY:  c = GOLD;   l = "H"; break;
        case ProjType::HOMING: c = GREEN;  l = "T"; break;
        case ProjType::SCATTER:c = PURPLE; l = "S"; break;
        default: continue;
        }
        DrawRectangle(startX + i * 16, startY, 13, 13, c);
        DrawText(l, startX + i * 16 + 3, startY + 1, 8, BLACK);
    }
}

void Game::DrawHUD() const
{
    DrawText(TextFormat("Score: %d", score), 10, 6, 22, WHITE);

    int bw = 140, bh = 12, bx = 10, by = 30;

    // P1 HP
    float r1 = (float)p1.hp / p1.maxHp;
    DrawRectangle(bx, by, bw, bh, DARKGRAY);
    DrawRectangle(bx, by, (int)(bw * r1), bh, BLUE);
    DrawRectangleLines(bx, by, bw, bh, LIGHTGRAY);
    DrawText(TextFormat("P1:%d", p1.hp), bx + 4, by, 12, WHITE);
    if (!p1.alive) DrawText("DEAD", bx + bw / 2 - 18, by, 12, RED);

    // P2 HP (only in 2P mode)
    if (twoPlayer) {
        int by2 = by + bh + 4;
        float r2 = (float)p2.hp / p2.maxHp;
        DrawRectangle(bx, by2, bw, bh, DARKGRAY);
        DrawRectangle(bx, by2, (int)(bw * r2), bh, SKYBLUE);
        DrawRectangleLines(bx, by2, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("P2:%d", p2.hp), bx + 4, by2, 12, WHITE);
        if (!p2.alive) DrawText("DEAD", bx + bw / 2 - 18, by2, 12, RED);
    }

    // Boss HP
    int eBx = Constants::SCREEN_WIDTH - bw - 10;
    if (!enemy.isDead) {
        float er = (float)enemy.hp / enemy.maxHp;
        DrawRectangle(eBx, by, bw, bh, DARKGRAY);
        DrawRectangle(eBx, by, (int)(bw * er), bh, RED);
        DrawRectangleLines(eBx, by, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("Boss:%d", enemy.hp), eBx + 4, by, 12, WHITE);
    } else {
        DrawText("BOSS DEFEATED", eBx, by, 14, GREEN);
    }
}
