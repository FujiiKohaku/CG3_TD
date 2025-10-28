#pragma once
#include "Sprite.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include <array>
#include <string>

class ScoreUI {
public:
    ScoreUI() = default;
    ~ScoreUI();

    void Initialize(SpriteManager* spriteManager);
    void Update(int score);
    void Draw();

private:
    std::array<Sprite*, 5> digits_; // 5桁表示
    SpriteManager* spriteManager_ = nullptr;
};
