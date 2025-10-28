#include "ScoreUI.h"

void ScoreUI::Initialize(SpriteManager* spriteManager)
{
    spriteManager_ = spriteManager;

    // 数字テクスチャ読み込み（0〜9）
    TextureManager::GetInstance()->LoadTexture("resources/0.png");
    TextureManager::GetInstance()->LoadTexture("resources/1.png");
    TextureManager::GetInstance()->LoadTexture("resources/2.png");
    TextureManager::GetInstance()->LoadTexture("resources/3.png");
    TextureManager::GetInstance()->LoadTexture("resources/4.png");
    TextureManager::GetInstance()->LoadTexture("resources/5.png");
    TextureManager::GetInstance()->LoadTexture("resources/6.png");
    TextureManager::GetInstance()->LoadTexture("resources/7.png");
    TextureManager::GetInstance()->LoadTexture("resources/8.png");
    TextureManager::GetInstance()->LoadTexture("resources/9.png");

    // 桁スプライトを初期化（全て0）
    for (int i = 0; i < 5; i++) {
        digits_[i] = new Sprite();
        digits_[i]->Initialize(spriteManager_, "resources/0.png");
        digits_[i]->SetSize({ 48, 48 });
        digits_[i]->SetPosition({ 970.0f + i * 55.0f, 50.0f });
    }
}

void ScoreUI::Update(int score)
{
    // スコアを5桁に分解
    int values[5] = {};
    for (int i = 4; i >= 0; i--) {
        values[i] = score % 10;
        score /= 10;
    }

    // 各桁の画像を更新
    for (int i = 0; i < 5; i++) {
        std::string path = "resources/" + std::to_string(values[i]) + ".png";
        digits_[i]->Initialize(spriteManager_, path);
        digits_[i]->Update();
    }
}

void ScoreUI::Draw()
{
    for (auto& d : digits_) {
        d->Draw();
    }
}

ScoreUI::~ScoreUI()
{
    for (auto& d : digits_) {
        delete d;
        d = nullptr;
    }
}
