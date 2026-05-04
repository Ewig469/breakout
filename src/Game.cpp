#include "Game.h"
#include "Constants.h"
#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
    : gameMode(GameMode::SINGLE), state(MODE_SELECT),
      bossSkillAccumulator(0), modeSelection(0),
      p1{Paddle(Constants::PADDLE1_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, 0, true},
      p2{Paddle(Constants::PADDLE2_Y), Ball(), {}, Constants::PLAYER_HP, Constants::PLAYER_HP, 0, true}
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

    const char* appDir = GetApplicationDirectory();
    texMenu     = LoadTexture(TextFormat("%spicture/menu.png", appDir));
    texGameOver = LoadTexture(TextFormat("%spicture/gameover.png", appDir));
    texVictory  = LoadTexture(TextFormat("%spicture/victory.png", appDir));

    while (!renderer.ShouldClose()) {
        float dt = GetFrameTime();
        renderer.BeginFrame();

        switch (state) {
        case MODE_SELECT: HandleModeSelect();  break;
        case MENU:        HandleMenu();        break;
        case PLAYING:
            if (gameMode == GameMode::VERSUS)
                HandlePlayingVersus(dt);
            else
                HandlePlayingCoop(dt);
            break;
        case GAME_OVER:   HandleGameOver();    break;
        case VICTORY:     HandleVictory();     break;
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

    DrawText("BREAKOUT", Constants::SCREEN_WIDTH / 2 - 100, 100, 50, WHITE);
    DrawText("Select Mode", Constants::SCREEN_WIDTH / 2 - 80, 170, 28, LIGHTGRAY);

    const char* options[3] = {"1 Player", "2 Players (Cooperative)", "2 Players (Versus)"};
    for (int i = 0; i < 3; ++i) {
        Color c = (modeSelection == i) ? YELLOW : GRAY;
        const char* prefix = (modeSelection == i) ? "> " : "  ";
        DrawText(TextFormat("%s%s", prefix, options[i]),
                 Constants::SCREEN_WIDTH / 2 - 140, 230 + i * 36, 22, c);
    }

    DrawText("UP / DOWN to choose  |  ENTER / SPACE to confirm",
             Constants::SCREEN_WIDTH / 2 - 190, 380, 16, DARKGRAY);

    if (input.UpPressed())   modeSelection = (modeSelection + 2) % 3;
    if (input.DownPressed()) modeSelection = (modeSelection + 1) % 3;

    if (input.P1Action()) {
        switch (modeSelection) {
        case 0: gameMode = GameMode::SINGLE; break;
        case 1: gameMode = GameMode::COOP;   break;
        case 2: gameMode = GameMode::VERSUS; break;
        }
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

    const char* title;
    switch (gameMode) {
    case GameMode::SINGLE: title = "BREAKOUT - 1 Player";        break;
    case GameMode::COOP:   title = "BREAKOUT - 2P Cooperative";  break;
    case GameMode::VERSUS: title = "BREAKOUT - 2P Versus";       break;
    }
    DrawText(title, Constants::SCREEN_WIDTH / 2 - 150, 130, 36, WHITE);
    DrawText("Press SPACE to start", Constants::SCREEN_WIDTH / 2 - 120, 200, 22, LIGHTGRAY);

    DrawText("P1 (bottom) :  A / D move  |  SPACE launch  |  F fire",
             60, 260, 16, GRAY);
    if (gameMode != GameMode::SINGLE) {
        const char* p2pos = (gameMode == GameMode::VERSUS) ? "(top)" : "";
        DrawText(TextFormat("P2 %s :  <- -> move  |  KP_1 launch  |  KP_2 fire", p2pos),
                 60, 286, 16, GRAY);
    }
    DrawText("Pickups: H=heavy(50)  T=homing(20)  S=scatter(30)  +=heal(20)",
             80, 330, 14, DARKGRAY);
    DrawText(">> = speed boost brick", 80, 350, 14, DARKGRAY);

    if (input.P1Action()) { ResetGame(); state = PLAYING; }
}

// ===== GAME OVER / VICTORY =====

void Game::HandleGameOver()
{
    DrawTexturePro(texGameOver, {0,0,(float)texGameOver.width,(float)texGameOver.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,80});

    DrawText("GAME OVER", Constants::SCREEN_WIDTH / 2 - 150, 200, 40, RED);

    if (gameMode == GameMode::VERSUS) {
        const char* winner = p1.alive ? "Player 1 Wins!" : "Player 2 Wins!";
        Color wc = p1.alive ? BLUE : RED;
        DrawText(winner, Constants::SCREEN_WIDTH / 2 - 100, 260, 30, wc);
        DrawText(TextFormat("P1 Score: %d    P2 Score: %d", p1.score, p2.score),
                 Constants::SCREEN_WIDTH / 2 - 140, 300, 18, WHITE);
    } else {
        DrawText(TextFormat("Final Score: %d", p1.score + p2.score),
                 Constants::SCREEN_WIDTH / 2 - 80, 260, 22, WHITE);
    }
    DrawText("Press SPACE to restart", Constants::SCREEN_WIDTH / 2 - 110, 350, 20, LIGHTGRAY);
    if (input.P1Action()) { ResetGame(); state = PLAYING; }
}

void Game::HandleVictory()
{
    DrawTexturePro(texVictory, {0,0,(float)texVictory.width,(float)texVictory.height},
                   {0,0,(float)Constants::SCREEN_WIDTH,(float)Constants::SCREEN_HEIGHT},
                   {0,0}, 0, WHITE);
    DrawRectangle(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, {0,0,0,80});

    DrawText("VICTORY!", Constants::SCREEN_WIDTH / 2 - 120, 200, 40, GREEN);

    if (gameMode == GameMode::VERSUS) {
        const char* winner;
        Color wc;
        if (p1.score > p2.score)      { winner = "Player 1 Wins!"; wc = BLUE; }
        else if (p2.score > p1.score) { winner = "Player 2 Wins!"; wc = RED; }
        else                          { winner = "Draw!";          wc = YELLOW; }
        DrawText(winner, Constants::SCREEN_WIDTH / 2 - 100, 260, 30, wc);
        DrawText(TextFormat("P1 Score: %d    P2 Score: %d", p1.score, p2.score),
                 Constants::SCREEN_WIDTH / 2 - 140, 300, 18, WHITE);
    } else {
        DrawText(TextFormat("Final Score: %d", p1.score + p2.score),
                 Constants::SCREEN_WIDTH / 2 - 80, 260, 22, WHITE);
    }
    DrawText("Press SPACE to play again", Constants::SCREEN_WIDTH / 2 - 130, 350, 20, LIGHTGRAY);
    if (input.P1Action()) { ResetGame(); state = PLAYING; }
}

// ===== COOP GAME LOOP =====

void Game::HandlePlayingCoop(float dt)
{
    if (p1.alive) p1.paddle.Update(dt, input.P1MoveDir());
    if (p2.alive) p2.paddle.Update(dt, input.P2MoveDir());

    if (p1.alive) HandleBall(p1, false);
    if (p2.alive) HandleBall(p2, true);

    if (state != PLAYING) return;
    if (!p1.alive && !p2.alive) { state = GAME_OVER; return; }
    if (brickGrid.AllCleared() && enemy.isDead) { state = VICTORY; return; }

    enemy.Update(dt);
    if (enemy.ConsumeShoot()) {
        float targetX = Constants::SCREEN_WIDTH / 2.0f;
        int cnt = (p1.alive ? 1 : 0) + (p2.alive ? 1 : 0);
        if (cnt > 0) {
            float sumX = 0;
            if (p1.alive) sumX += p1.paddle.rect.x + p1.paddle.rect.width / 2.0f;
            if (p2.alive) sumX += p2.paddle.rect.x + p2.paddle.rect.width / 2.0f;
            targetX = sumX / cnt;
        }
        float dx = targetX - (enemy.rect.x + enemy.rect.width / 2.0f);
        float len = std::sqrt(dx*dx + Constants::SCREEN_HEIGHT*(float)Constants::SCREEN_HEIGHT);
        if (len > 0) dx /= len;
        Projectile p;
        p.Fire(enemy.GetShootPos(),
               {dx * Constants::ENEMY_PROJECTILE_SPEED * 0.4f, Constants::ENEMY_PROJECTILE_SPEED},
               ProjType::ENEMY, 3);
        enemyProjectiles.push_back(p);
    }

    for (auto& p : enemyProjectiles) p.Update(dt);

    Vector2 enemyCenter = {enemy.rect.x + enemy.rect.width/2, enemy.rect.y + enemy.rect.height/2};
    for (auto& p : p1Projectiles) {
        if (p.type == ProjType::HOMING && !enemy.isDead) p.UpdateHoming(dt, enemyCenter);
        p.Update(dt);
    }

    for (auto& pu : powerUps) pu.Update(dt);

    for (auto& p : enemyProjectiles) {
        if (!p.active) continue;
        if (p1.alive && CheckCollisionCircleRec(p.position, p.size, p1.paddle.rect))
            { ApplyPlayerDamage(p1, p.damage); p.Deactivate(); continue; }
        if (p2.alive && CheckCollisionCircleRec(p.position, p.size, p2.paddle.rect))
            { ApplyPlayerDamage(p2, p.damage); p.Deactivate(); }
    }
    if (!p1.alive && !p2.alive) { state = GAME_OVER; return; }

    if (!enemy.isDead) {
        for (auto& p : p1Projectiles) {
            if (!p.active || p.type == ProjType::SCATTER_FRAG) continue;
            if (CheckCollisionCircleRec(p.position, p.size, enemy.rect)) {
                enemy.TakeDamage(p.damage);
                if (p.type == ProjType::SCATTER) HandleScatter(p.position, p.ownerId);
                p.Deactivate();
                if (enemy.isDead && brickGrid.AllCleared()) { state = VICTORY; return; }
            }
        }
    }

    for (auto& p : p1Projectiles) {
        if (!p.active || p.type != ProjType::SCATTER_FRAG) continue;
        int pts = brickGrid.CheckCircleCollision(p.position, p.size);
        if (pts > 0) { p1.score += pts; p.Deactivate();
            for (auto& pos : brickGrid.DrainDropPositions()) SpawnPowerUp(pos); }
    }

    for (auto& pu : powerUps) {
        if (!pu.active) continue;
        Rectangle r = {pu.position.x-8, pu.position.y-8, 16, 16};
        if (p1.alive && CheckCollisionRecs(r, p1.paddle.rect)) { CollectPowerUp(p1,pu); continue; }
        if (p2.alive && CheckCollisionRecs(r, p2.paddle.rect)) { CollectPowerUp(p2,pu); }
    }

    if (p1.alive && input.P1Fire() && !p1.inventory.empty())
        { ProjType t = p1.inventory.front(); p1.inventory.erase(p1.inventory.begin());
          FirePlayerProjectile(t, {p1.paddle.rect.x + p1.paddle.rect.width/2, p1.paddle.rect.y}, 1); }
    if (p2.alive && input.P2Fire() && !p2.inventory.empty())
        { ProjType t = p2.inventory.front(); p2.inventory.erase(p2.inventory.begin());
          FirePlayerProjectile(t, {p2.paddle.rect.x + p2.paddle.rect.width/2, p2.paddle.rect.y}, 2); }

    CleanupInactive();

    brickGrid.Draw();
    enemy.Draw();
    if (p1.alive) DrawPlayer(p1, BLUE);
    if (p2.alive) DrawPlayer(p2, SKYBLUE);
    for (auto& p : enemyProjectiles) p.Draw();
    for (auto& p : p1Projectiles) p.Draw();
    for (auto& pu : powerUps) pu.Draw();
    DrawHUD();
}

// ===== VERSUS GAME LOOP =====

void Game::HandlePlayingVersus(float dt)
{
    // Paddles
    if (p1.alive) p1.paddle.Update(dt, input.P1MoveDir());
    if (p2.alive) p2.paddle.Update(dt, input.P2MoveDir());

    // P1 ball (bottom, normal walls)
    if (p1.alive) HandleBallVersus(p1, false);
    // P2 ball (top, inverted walls)
    if (p2.alive) HandleBallVersus(p2, true);

    if (state != PLAYING) return;
    if (!p1.alive || !p2.alive) { state = GAME_OVER; return; }
    if (brickGrid.AllCleared()) { state = VICTORY; return; }

    // Update projectiles
    Vector2 p1Center = {p1.paddle.rect.x + p1.paddle.rect.width/2,
                        p1.paddle.rect.y + p1.paddle.rect.height/2};
    Vector2 p2Center = {p2.paddle.rect.x + p2.paddle.rect.width/2,
                        p2.paddle.rect.y + p2.paddle.rect.height/2};
    for (auto& p : p1Projectiles) {
        if (p.type == ProjType::HOMING && p2.alive) p.UpdateHoming(dt, p2Center);
        p.Update(dt);
    }
    for (auto& p : p2Projectiles) {
        if (p.type == ProjType::HOMING && p1.alive) p.UpdateHoming(dt, p1Center);
        p.Update(dt);
    }

    for (auto& pu : powerUps) pu.Update(dt);

    // P1 projectiles hit P2
    for (auto& p : p1Projectiles) {
        if (!p.active || p.type == ProjType::SCATTER_FRAG) continue;
        if (p2.alive && CheckCollisionCircleRec(p.position, p.size, p2.paddle.rect)) {
            ApplyPlayerDamage(p2, p.damage);
            if (p.type == ProjType::SCATTER) HandleScatter(p.position, p.ownerId);
            p.Deactivate();
            if (!p2.alive) { state = GAME_OVER; return; }
        }
    }
    // P2 projectiles hit P1
    for (auto& p : p2Projectiles) {
        if (!p.active || p.type == ProjType::SCATTER_FRAG) continue;
        if (p1.alive && CheckCollisionCircleRec(p.position, p.size, p1.paddle.rect)) {
            ApplyPlayerDamage(p1, p.damage);
            if (p.type == ProjType::SCATTER) HandleScatter(p.position, p.ownerId);
            p.Deactivate();
            if (!p1.alive) { state = GAME_OVER; return; }
        }
    }

    // Scatter fragments hit bricks
    for (auto& p : p1Projectiles) {
        if (!p.active || p.type != ProjType::SCATTER_FRAG) continue;
        int pts = brickGrid.CheckCircleCollision(p.position, p.size);
        if (pts > 0) { p1.score += pts; p.Deactivate();
            for (auto& pos : brickGrid.DrainDropPositions()) SpawnPowerUp(pos); }
    }
    for (auto& p : p2Projectiles) {
        if (!p.active || p.type != ProjType::SCATTER_FRAG) continue;
        int pts = brickGrid.CheckCircleCollision(p.position, p.size);
        if (pts > 0) { p2.score += pts; p.Deactivate();
            for (auto& pos : brickGrid.DrainDropPositions()) SpawnPowerUp(pos); }
    }

    // Power-ups
    for (auto& pu : powerUps) {
        if (!pu.active) continue;
        Rectangle r = {pu.position.x-8, pu.position.y-8, 16, 16};
        if (p1.alive && CheckCollisionRecs(r, p1.paddle.rect)) { CollectPowerUp(p1,pu); continue; }
        if (p2.alive && CheckCollisionRecs(r, p2.paddle.rect)) { CollectPowerUp(p2,pu); }
    }

    // Fire
    if (p1.alive && input.P1Fire() && !p1.inventory.empty())
        { ProjType t = p1.inventory.front(); p1.inventory.erase(p1.inventory.begin());
          FirePlayerProjectile(t, {p1.paddle.rect.x + p1.paddle.rect.width/2, p1.paddle.rect.y}, 1); }
    if (p2.alive && input.P2Fire() && !p2.inventory.empty())
        { ProjType t = p2.inventory.front(); p2.inventory.erase(p2.inventory.begin());
          FirePlayerProjectile(t, {p2.paddle.rect.x + p2.paddle.rect.width/2, p2.paddle.rect.y}, 2); }

    CleanupInactive();

    brickGrid.Draw();
    if (p1.alive) DrawPlayer(p1, BLUE);
    if (p2.alive) DrawPlayer(p2, RED);
    for (auto& p : p1Projectiles) p.Draw();
    for (auto& p : p2Projectiles) p.Draw();
    for (auto& pu : powerUps) pu.Draw();
    DrawHUD();
}

// ===== BALL HANDLERS =====

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
    CheckPaddleCollision(pl.ball, pl.paddle);
    { PlayerState& other = isP2 ? p1 : p2;
      if (other.alive) CheckPaddleCollision(pl.ball, other.paddle); }
    CheckBallEnemyCollision(pl.ball);

    int pts = brickGrid.CheckCollision(pl.ball);
    p1.score += pts;
    if (brickGrid.ConsumeSpeedBoost()) pl.ball.ApplySpeedBoost();

    auto drops = brickGrid.DrainDropPositions();
    for (auto& pos : drops) SpawnPowerUp(pos);

    if (pl.ball.position.y > Constants::SCREEN_HEIGHT) {
        ApplyPlayerDamage(pl, Constants::BALL_FALL_DAMAGE);
        if (pl.alive) pl.ball.Reset();
    }
}

void Game::HandleBallVersus(PlayerState& pl, bool isP2)
{
    if (!pl.ball.IsLaunched()) {
        pl.ball.position.x = pl.paddle.rect.x + pl.paddle.rect.width / 2;
        if (isP2)
            pl.ball.position.y = pl.paddle.rect.y + pl.paddle.rect.height + pl.ball.radius + 1;
        else
            pl.ball.position.y = pl.paddle.rect.y - pl.ball.radius - 1;

        bool launch = isP2 ? input.P2Action() : input.P1Action();
        if (launch)
            pl.ball.Launch(isP2 ? 1.0f : -1.0f);
        return;
    }

    pl.ball.Update(GetFrameTime());

    // Own paddle
    CheckPaddleCollision(pl.ball, pl.paddle);

    // Opponent paddle — ball deals damage on hit
    PlayerState& opp = isP2 ? p1 : p2;
    if (opp.alive && CheckPaddleCollision(pl.ball, opp.paddle)) {
        ApplyPlayerDamage(opp, 10);
        pl.score += 5;
    }

    // Brick collisions
    int pts = brickGrid.CheckCollision(pl.ball);
    pl.score += pts;
    if (brickGrid.ConsumeSpeedBoost()) pl.ball.ApplySpeedBoost();

    auto drops = brickGrid.DrainDropPositions();
    for (auto& pos : drops) SpawnPowerUp(pos);

    // Death zone: each ball only dies at its OWN bottom
    if (!isP2 && pl.ball.position.y > Constants::SCREEN_HEIGHT) {
        ApplyPlayerDamage(pl, Constants::BALL_FALL_DAMAGE);
        if (pl.alive) pl.ball.Reset();
    }
    if (isP2 && pl.ball.position.y + pl.ball.radius < 0) {
        ApplyPlayerDamage(pl, Constants::BALL_FALL_DAMAGE);
        if (pl.alive) pl.ball.Reset();
    }
}

// ===== DAMAGE =====

void Game::ApplyPlayerDamage(PlayerState& pl, int damage)
{
    pl.hp -= damage;
    if (gameMode != GameMode::VERSUS) {
        bossSkillAccumulator += damage;
        while (bossSkillAccumulator >= Constants::BOSS_SKILL_HP_THRESHOLD) {
            bossSkillAccumulator -= Constants::BOSS_SKILL_HP_THRESHOLD;
            if (!enemy.isDead)
                brickGrid.ReviveRandomBricks(Constants::BOSS_SKILL_BRICK_COUNT);
        }
    }
    if (pl.hp <= 0) { pl.hp = 0; pl.alive = false; }
}

// ===== PADDLE COLLISION =====

bool Game::CheckPaddleCollision(Ball& b, const Paddle& pad)
{
    if (!CheckCollisionCircleRec(b.position, b.radius, pad.rect))
        return false;

    bool padIsTop = (pad.rect.y < Constants::SCREEN_HEIGHT / 2.0f);

    // Ball must be moving TOWARD the pad
    if (padIsTop) {
        if (b.velocity.y > 0) return false;  // moving down = away from top pad
    } else {
        if (b.velocity.y < 0) return false;  // moving up   = away from bottom pad
    }

    float hitPos = (b.position.x - pad.rect.x) / pad.rect.width;
    float angle  = (hitPos - 0.5f) * PI;
    float maxA   = Constants::BALL_MAX_BOUNCE_ANGLE;
    if (angle >  maxA) angle =  maxA;
    if (angle < -maxA) angle = -maxA;

    float spd = std::sqrt(b.velocity.x * b.velocity.x + b.velocity.y * b.velocity.y);
    b.velocity.x = spd * std::sin(angle);

    if (padIsTop) {
        b.velocity.y =  std::abs(spd * std::cos(angle));   // bounce DOWN
        b.position.y = pad.rect.y + pad.rect.height + b.radius;
    } else {
        b.velocity.y = -std::abs(spd * std::cos(angle));   // bounce UP
        b.position.y = pad.rect.y - b.radius;
    }
    return true;
}

void Game::CheckBallEnemyCollision(Ball& b)
{
    if (enemy.isDead) return;
    if (!CheckCollisionCircleRec(b.position, b.radius, enemy.rect)) return;

    enemy.TakeDamage(Constants::BALL_ENEMY_DAMAGE);

    float hitPos = (b.position.x - enemy.rect.x) / enemy.rect.width;
    float angle  = (hitPos - 0.5f) * PI * 0.7f;
    float spd    = std::sqrt(b.velocity.x*b.velocity.x + b.velocity.y*b.velocity.y);
    b.velocity.x = spd * std::sin(angle);
    b.velocity.y = spd * std::cos(angle);
    if (b.velocity.y < 80.0f) b.velocity.y = 120.0f;
    b.position.y = enemy.rect.y + enemy.rect.height + b.radius;
}

// ===== POWER-UP / FIRE =====

void Game::SpawnPowerUp(Vector2 pos)
{
    PowerUp pu;
    int r = std::rand() % 100;
    PickupType t;
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

void Game::FirePlayerProjectile(ProjType type, Vector2 origin, int ownerId)
{
    Projectile p;
    bool fromTop = (gameMode == GameMode::VERSUS && ownerId == 2);
    float yDir = fromTop ? 1.0f : -1.0f;

    switch (type) {
    case ProjType::HEAVY:
        p.Fire(origin, {0, yDir * Constants::HEAVY_PROJECTILE_SPEED}, ProjType::HEAVY, ownerId);
        break;
    case ProjType::HOMING: {
        Vector2 tgt;
        if (gameMode == GameMode::VERSUS) {
            PlayerState& trg = (ownerId == 1) ? p2 : p1;
            tgt = {trg.paddle.rect.x + trg.paddle.rect.width/2,
                   trg.paddle.rect.y + trg.paddle.rect.height/2};
        } else {
            tgt = {enemy.rect.x + enemy.rect.width/2, enemy.rect.y + enemy.rect.height/2};
        }
        Vector2 dir = {tgt.x - origin.x, tgt.y - origin.y};
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (len > 0) { dir.x /= len; dir.y /= len; }
        p.Fire(origin,
               {dir.x * Constants::HOMING_PROJECTILE_SPEED,
                dir.y * Constants::HOMING_PROJECTILE_SPEED},
               ProjType::HOMING, ownerId);
        break;
    }
    case ProjType::SCATTER:
        p.Fire(origin, {0, yDir * Constants::SCATTER_PROJECTILE_SPEED}, ProjType::SCATTER, ownerId);
        break;
    default: return;
    }

    if (gameMode == GameMode::VERSUS) {
        if (ownerId == 1) p1Projectiles.push_back(p);
        else              p2Projectiles.push_back(p);
    } else {
        p1Projectiles.push_back(p);
    }
}

void Game::HandleScatter(Vector2 pos, int ownerId)
{
    for (int i = 0; i < 3; ++i) {
        Projectile frag;
        float angle = -0.9f + 0.9f * i;
        Vector2 vel = {std::sin(angle) * Constants::SCATTER_FRAG_SPEED,
                       std::cos(angle) * Constants::SCATTER_FRAG_SPEED};
        frag.Fire(pos, vel, ProjType::SCATTER_FRAG, ownerId);
        if (gameMode == GameMode::VERSUS) {
            if (ownerId == 1) p1Projectiles.push_back(frag);
            else              p2Projectiles.push_back(frag);
        } else {
            p1Projectiles.push_back(frag);
        }
    }
}

void Game::CleanupInactive()
{
    auto rm = [](std::vector<Projectile>& v) {
        v.erase(std::remove_if(v.begin(), v.end(),
                 [](const Projectile& p) { return !p.active; }), v.end());
    };
    rm(enemyProjectiles);
    rm(p1Projectiles);
    rm(p2Projectiles);
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
                   [](const PowerUp& pu) { return !pu.active; }), powerUps.end());
}

void Game::ResetGame()
{
    bossSkillAccumulator = 0;

    float p2y = (gameMode == GameMode::VERSUS) ? Constants::PADDLE_VERSUS_Y : Constants::PADDLE2_Y;
    int php = (gameMode == GameMode::VERSUS) ? Constants::VERSUS_HP : Constants::PLAYER_HP;
    p1 = {Paddle(Constants::PADDLE1_Y), Ball(), {}, php, php, 0, true};
    p2 = {Paddle(p2y),                  Ball(), {}, php, php, 0, true};

    // Ball colors
    p1.ball.ballColor = RED;
    p2.ball.ballColor = BLUE;

    // P2 invertWalls in versus mode
    if (gameMode == GameMode::VERSUS) {
        p2.ball.invertWalls = true;
    }

    enemyProjectiles.clear();
    p1Projectiles.clear();
    p2Projectiles.clear();
    powerUps.clear();

    if (gameMode == GameMode::VERSUS) {
        // Center bricks between paddles: P2 at y~20, P1 at y~550
        float midY = (Constants::PADDLE_VERSUS_Y + Constants::PADDLE1_Y) / 2.0f;
        float brickH = Constants::BRICK_ROWS * (Constants::BRICK_HEIGHT + Constants::BRICK_PADDING) - Constants::BRICK_PADDING;
        float yOff = midY - brickH / 2.0f;
        brickGrid.Generate(Constants::BRICK_ROWS, Constants::BRICK_COLS, yOff, true);
    } else {
        brickGrid.Reset();
    }
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
    int bw = 140, bh = 12, bx = 10, by = 30;

    if (gameMode == GameMode::VERSUS) {
        float r1 = (float)p1.hp / p1.maxHp;
        DrawRectangle(bx, by, bw, bh, DARKGRAY);
        DrawRectangle(bx, by, (int)(bw * r1), bh, BLUE);
        DrawRectangleLines(bx, by, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("P1:%d S:%d", p1.hp, p1.score), bx+4, by, 12, WHITE);
        if (!p1.alive) DrawText("DEAD", bx+bw/2-18, by, 12, RED);

        int b2x = Constants::SCREEN_WIDTH - bw - 10;
        float r2 = (float)p2.hp / p2.maxHp;
        DrawRectangle(b2x, by, bw, bh, DARKGRAY);
        DrawRectangle(b2x, by, (int)(bw * r2), bh, RED);
        DrawRectangleLines(b2x, by, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("P2:%d S:%d", p2.hp, p2.score), b2x+4, by, 12, WHITE);
        if (!p2.alive) DrawText("DEAD", b2x+bw/2-18, by, 12, RED);

        DrawText("VS", Constants::SCREEN_WIDTH/2-16, by, 20, YELLOW);
        return;
    }

    float r1 = (float)p1.hp / p1.maxHp;
    DrawRectangle(bx, by, bw, bh, DARKGRAY);
    DrawRectangle(bx, by, (int)(bw * r1), bh, BLUE);
    DrawRectangleLines(bx, by, bw, bh, LIGHTGRAY);
    DrawText(TextFormat("P1:%d", p1.hp), bx+4, by, 12, WHITE);
    if (!p1.alive) DrawText("DEAD", bx+bw/2-18, by, 12, RED);

    if (gameMode == GameMode::COOP) {
        int by2 = by + bh + 4;
        float r2 = (float)p2.hp / p2.maxHp;
        DrawRectangle(bx, by2, bw, bh, DARKGRAY);
        DrawRectangle(bx, by2, (int)(bw * r2), bh, SKYBLUE);
        DrawRectangleLines(bx, by2, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("P2:%d", p2.hp), bx+4, by2, 12, WHITE);
        if (!p2.alive) DrawText("DEAD", bx+bw/2-18, by2, 12, RED);
    }

    DrawText(TextFormat("Score: %d", p1.score), Constants::SCREEN_WIDTH-bw-10, 6, 22, WHITE);

    int eBx = Constants::SCREEN_WIDTH - bw - 10;
    if (!enemy.isDead) {
        float er = (float)enemy.hp / enemy.maxHp;
        DrawRectangle(eBx, by, bw, bh, DARKGRAY);
        DrawRectangle(eBx, by, (int)(bw * er), bh, RED);
        DrawRectangleLines(eBx, by, bw, bh, LIGHTGRAY);
        DrawText(TextFormat("Boss:%d", enemy.hp), eBx+4, by, 12, WHITE);
    } else {
        DrawText("BOSS DEFEATED", eBx, by, 14, GREEN);
    }
}
