#include "Stage2Logic.h"
#include "Coin.h"
#include "Player.h"

void Stage2Logic::Initialize()
{
    // コインを縦×横に生成
    for (int y = 0; y < 15; y++) { // 縦方向（段）
        for (int x = 0; x < 12; x++) { // 横方向（列）
            Coin* coin = new Coin();

            // 左下に移動
            Vector3 pos = { -17.0f + x * 3.0f, -16.0f + y * 2.5f, 0.0f };

            coin->Initialize(pos, 1.0f, 5, object3dManager_, "Coin.obj");
            coins_.push_back(coin);
        }
    }

    //----------------------------------------
    //  サウンド
    //----------------------------------------
    sound_ = new SoundManager();
    sound_->Initialize();
    coin_ = sound_->SoundLoadWave("resources/coin.wav");
}

void Stage2Logic::Update()
{
    // プレイヤーの球の情報を取得
    Sphere playerSphere = { pendulumPlayer_->GetPosition(), pendulumPlayer_->GetRadius() };
    // pendulum が「切れていない」＝まだ振り子で揺れている間はコインを取得できない
    if (pendulumPlayer_->GetPendulum()->GetIsCut()) {
        // 慣性移動中のみコイン判定を行う
        for (auto& coin : coins_) {
            coin->Update();
            if (coin->IsCollision(playerSphere)) {
                pendulumPlayer_->AddScore(coin->GetScore());
                sound_->SoundPlayWave(coin_, false);
            }
        }
    } else {
        // まだロープでつながっている間（スイング中）はコインだけ更新しておく
        for (auto& coin : coins_) {
            coin->Update();
        }
    }
}

void Stage2Logic::Draw()
{
    for (auto& coin : coins_) {
        coin->Draw();
    }
}

void Stage2Logic::Finalize()
{
    for (auto& coin : coins_) {
        delete coin;
    }
    // ===== サウンド解放 =====
    if (sound_) {
        sound_->SoundUnload(&coin_);
        sound_->Finalize(nullptr);
        delete sound_;
        sound_ = nullptr;
    }
    coins_.clear();
}
