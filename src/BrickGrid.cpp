#include "BrickGrid.h"
#include "Constants.h"
#include <cstdlib>
#include <vector>
#include <utility>

BrickGrid::BrickGrid()
{
    Generate(Constants::BRICK_ROWS, Constants::BRICK_COLS);
}

void BrickGrid::Generate(int r, int c)
{
    rows = r;
    cols = c;
    grid.clear();
    grid.resize(rows);

    for (int i = 0; i < rows; ++i) {
        grid[i].reserve(cols);
        for (int j = 0; j < cols; ++j) {
            float x = Constants::BRICK_OFFSET_LEFT + j * (Constants::BRICK_WIDTH + Constants::BRICK_PADDING);
            float y = Constants::BRICK_OFFSET_TOP  + i * (Constants::BRICK_HEIGHT + Constants::BRICK_PADDING);
            int   hp = (rows - i + 2) / 3 + 1;
            if (hp > 3) hp = 3;
            grid[i].emplace_back(x, y, Constants::BRICK_WIDTH, Constants::BRICK_HEIGHT, hp);
        }
    }
}

void BrickGrid::Draw() const
{
    for (const auto& row : grid)
        for (const auto& brick : row)
            brick.Draw();
}

int BrickGrid::CheckCollision(Ball& ball)
{
    for (auto& row : grid) {
        for (auto& brick : row) {
            if (brick.destroyed) continue;

            if (CheckCollisionCircleRec(ball.position, ball.radius, brick.rect)) {
                int points = brick.Hit();

                float ballLeft   = ball.position.x - ball.radius;
                float ballRight  = ball.position.x + ball.radius;
                float ballTop    = ball.position.y - ball.radius;
                float ballBottom = ball.position.y + ball.radius;

                float overlapLeft   = ballRight  - brick.rect.x;
                float overlapRight  = (brick.rect.x + brick.rect.width) - ballLeft;
                float overlapTop    = ballBottom - brick.rect.y;
                float overlapBottom = (brick.rect.y + brick.rect.height) - ballTop;

                float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
                float minOverlapY = (overlapTop  < overlapBottom) ? overlapTop  : overlapBottom;

                if (minOverlapX < minOverlapY)
                    ball.velocity.x = -ball.velocity.x;
                else
                    ball.velocity.y = -ball.velocity.y;

                if (brick.destroyed)
                    MaybeAddDrop(brick);

                return points;
            }
        }
    }
    return 0;
}

int BrickGrid::CheckCircleCollision(Vector2 center, float radius)
{
    for (auto& row : grid) {
        for (auto& brick : row) {
            if (brick.destroyed) continue;

            if (CheckCollisionCircleRec(center, radius, brick.rect)) {
                int points = brick.Hit();
                if (brick.destroyed)
                    MaybeAddDrop(brick);
                return points;
            }
        }
    }
    return 0;
}

bool BrickGrid::AllCleared() const
{
    for (const auto& row : grid)
        for (const auto& brick : row)
            if (!brick.destroyed)
                return false;
    return true;
}

void BrickGrid::ReviveRandomBricks(int count)
{
    // Collect indices of all destroyed bricks
    std::vector<std::pair<int,int>> destroyed;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (grid[i][j].destroyed)
                destroyed.push_back({i, j});

    int revived = 0;
    while (revived < count && !destroyed.empty()) {
        int idx = rand() % destroyed.size();
        auto [i, j] = destroyed[idx];
        destroyed.erase(destroyed.begin() + idx);

        float x = Constants::BRICK_OFFSET_LEFT + j * (Constants::BRICK_WIDTH + Constants::BRICK_PADDING);
        float y = Constants::BRICK_OFFSET_TOP  + i * (Constants::BRICK_HEIGHT + Constants::BRICK_PADDING);
        int hp = 1 + rand() % 2;  // 1 or 2 HP

        grid[i][j] = Brick(x, y, Constants::BRICK_WIDTH, Constants::BRICK_HEIGHT, hp);
        revived++;
    }
}

void BrickGrid::Reset()
{
    dropPositions.clear();
    Generate(Constants::BRICK_ROWS, Constants::BRICK_COLS);
}

std::vector<Vector2> BrickGrid::DrainDropPositions()
{
    std::vector<Vector2> result;
    result.swap(dropPositions);
    return result;
}

void BrickGrid::MaybeAddDrop(const Brick& brick)
{
    float roll = (float)rand() / RAND_MAX;
    if (roll < Constants::POWERUP_DROP_CHANCE) {
        Vector2 center = {
            brick.rect.x + brick.rect.width / 2.0f,
            brick.rect.y + brick.rect.height / 2.0f
        };
        dropPositions.push_back(center);
    }
}
