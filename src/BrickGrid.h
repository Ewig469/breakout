#pragma once
#include <vector>
#include "Brick.h"
#include "Ball.h"
#include "raylib.h"

class BrickGrid {
public:
    BrickGrid();

    void Generate(int rows, int cols);
    void Draw() const;
    int  CheckCollision(Ball& ball);
    int  CheckCircleCollision(Vector2 center, float radius);
    bool AllCleared() const;
    void ReviveRandomBricks(int count);
    void Reset();

    std::vector<Vector2> DrainDropPositions();

private:
    std::vector<std::vector<Brick>> grid;
    std::vector<Vector2> dropPositions;
    int rows = 0;
    int cols = 0;

    void MaybeAddDrop(const Brick& brick);
};
