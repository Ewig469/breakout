#include "Brick.h"
#include "Constants.h"

static Texture2D texWood   = {0};
static Texture2D texStone  = {0};
static Texture2D texIron   = {0};
static Texture2D texBlitz  = {0};
static bool texturesLoaded = false;

Brick::Brick(float x, float y, float width, float height, int hp, bool speedBoost)
    : rect{x, y, width, height}, hp(hp), destroyed(false), isSpeedBoost(speedBoost),
      initialHp(hp)
{
}

void Brick::LoadTextures()
{
    if (texturesLoaded) return;
    const char* appDir = GetApplicationDirectory();
    texWood  = LoadTexture(TextFormat("%spicture/brickwood.png",  appDir));
    texStone = LoadTexture(TextFormat("%spicture/brickstone.png", appDir));
    texIron  = LoadTexture(TextFormat("%spicture/brickiron.png",  appDir));
    texBlitz = LoadTexture(TextFormat("%spicture/blitz.png",      appDir));
    texturesLoaded = true;
}

void Brick::UnloadTextures()
{
    UnloadTexture(texWood);
    UnloadTexture(texStone);
    UnloadTexture(texIron);
    UnloadTexture(texBlitz);
    texturesLoaded = false;
}

int Brick::Hit()
{
    hp--;
    if (hp <= 0) {
        destroyed = true;
        return initialHp * 10;
    }
    return 0;
}

void Brick::Draw() const
{
    if (destroyed) return;

    if (texturesLoaded) {
        Texture2D tex;
        if (isSpeedBoost)
            tex = texBlitz;
        else switch (hp) {
        case 3: tex = texIron;  break;
        case 2: tex = texStone; break;
        default:tex = texWood;  break;
        }
        DrawTexturePro(tex,
                       {0, 0, (float)tex.width, (float)tex.height},
                       rect,
                       {0, 0}, 0, WHITE);
    } else {
        // Fallback: colored rectangles
        Color c;
        if (isSpeedBoost)
            c = {0, 255, 255, 255};
        else switch (hp) {
        case 3: c = RED;    break;
        case 2: c = ORANGE; break;
        default:c = YELLOW; break;
        }
        DrawRectangleRec(rect, c);
        if (isSpeedBoost)
            DrawText(">>", rect.x + rect.width/2 - 10, rect.y + 5, 10, BLACK);
    }

    // Subtle border
    DrawRectangleLinesEx(rect, 1, {0, 0, 0, 80});
}
