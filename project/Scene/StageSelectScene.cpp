#include "StageSelectScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "Fade.h"

void StageSelectScene::Initialize() {
	fade_ = new Fade();
	fade_->Initialize(GetDx());
	fade_->Start(Status::FadeIn, 0.25f);
}

void StageSelectScene::Update(Input* input) {
	if (fade_) { fade_->Update(); }

	switch (phase_) {
	case Phase::kFadeIn:
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (input->IsKeyTriggered(DIK_LEFT)) {
			selected_ = (selected_ - 1 + int(StageId::Count)) % int(StageId::Count);
		}
		if (input->IsKeyTriggered(DIK_RIGHT)) {
			selected_ = (selected_ + 1) % int(StageId::Count);
		}
		if (input->IsKeyTriggered(DIK_SPACE)) {

			fade_->Start(Status::FadeOut, 0.25f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		// フェードアウト処理
		if (fade_->IsFinished()) {
			// シーン切り替え
			SceneParam param{};
			param.stage = static_cast<StageId>(selected_);
			GetSceneManager()->SetNextScene(new GamePlayScene(), param);
		}
		break;
	}


}

void StageSelectScene::Draw() {
	GetDx()->PreDraw();

	fade_->Draw();

	GetDx()->PostDraw();
}

void StageSelectScene::Finalize() {
	delete fade_;
	fade_ = nullptr;
}
