#include "Stage1Logic.h"
#include "WarpGate.h"

void Stage1Logic::Initialize() {
	warpA_ = new WarpGate();
	warpA_->Initialize(kWarpAPosition, kWarpScale, object3dManager_, kWarpModel);

	warpB_ = new WarpGate();
	warpB_->Initialize(kWarpBPosition, kWarpScale, object3dManager_, kWarpModel);

	// お互いをリンク（ペア設定）
	warpA_->SetPair(warpB_);
	warpB_->SetPair(warpA_);
}

void Stage1Logic::Update() {
	// ステージ1：ワープゲート＋基本プレイ
	if (warpA_ && warpB_) {
		warpA_->Update();
		warpB_->Update();
		warpA_->CheckAndWarp(pendulumPlayer_);
		warpB_->CheckAndWarp(pendulumPlayer_);
	}
}

void Stage1Logic::Draw() {
	warpA_->Draw();
	warpB_->Draw();
}

void Stage1Logic::Finalize() {
	delete warpA_;
	warpA_ = nullptr;
	delete warpB_;
	warpB_ = nullptr;
}
