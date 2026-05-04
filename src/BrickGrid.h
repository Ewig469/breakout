#pragma once
#include <vector>
#include "Brick.h"
#include "Ball.h"
#include "raylib.h"

class BrickGrid {
public:
    BrickGrid();

    void Generate(int rows, int cols, float yOffset = -1.0f, bool symmetric = false);
    void Draw() const;
    int  CheckCollision(Ball& ball);
    int  CheckCircleCollision(Vector2 center, float radius);
    bool AllCleared() const;
    void ReviveRandomBricks(int count);
    void Reset();
    bool ConsumeSpeedBoost();

    std::vector<Vector2> DrainDropPositions();

private:
    std::vector<std::vector<Brick>> grid;
    std::vector<Vector2> dropPositions;
    bool speedBoostTriggered = false;
    int rows = 0;
    int cols = 0;
    float currentYOffset = 0;

    void MaybeAddDrop(const Brick& brick);
};
