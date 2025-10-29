#include "Stage3Logic.h"
#include "Player.h"
#include "Coin.h"

void Stage3Logic::Initialize() {
    for (int i = 0; i < 8; i++) { // 縦方向（段）
            Coin* coin = new Coin();

            // 左下に移動
            Vector3 pos = { -17.5f + i * 5.0f, -17.5f + i * 5.0f, 0.0f };
            coin->Initialize(pos, 1.0f, 10, object3dManager_, "Coin.obj");
			coin->SetVelocity({ 0.1f,0.0f,0.0f });
            coins_.push_back(coin);
        
    }
}

void Stage3Logic::Update() {

	// プレイヤーの球の情報を取得
	Sphere playerSphere = { pendulumPlayer_->GetPosition(), pendulumPlayer_->GetRadius() };
	// pendulum が「切れていない」＝まだ振り子で揺れている間はコインを取得できない
	if (pendulumPlayer_->GetPendulum()->GetIsCut()) {
		// 慣性移動中のみコイン判定を行う
		for (auto& coin : coins_) {
			coin->Update();
			if (coin->IsCollision(playerSphere)) {
				pendulumPlayer_->AddScore(coin->GetScore());
			}
		}
	} else {
		// まだロープでつながっている間（スイング中）はコインだけ更新しておく
		for (auto& coin : coins_) {
			coin->Update();
		}
	}
}

void Stage3Logic::Draw() {
	for (auto& coin : coins_) {
		coin->Draw();
	}
}

void Stage3Logic::Finalize() {
	for (auto& coin : coins_) {
		delete coin;
	}
	coins_.clear();
}
