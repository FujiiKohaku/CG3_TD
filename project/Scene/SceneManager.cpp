#include "SceneManager.h"

void SceneManager::Update(Input* input)
{

	// 次シーンの予約があるなら
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		// シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);
		scene_->SetSystem(winApp_, dxCommon_);
		// 次シーンを初期化
		scene_->Initialize();
	}

	if (!scene_) { return; }

	scene_->Update(input);
}

void SceneManager::Draw()
{
	scene_->Draw();
}

SceneManager::~SceneManager() {
	// 最期のシーンの終了と解放
	scene_->Finalize();
	delete scene_;
	scene_ = nullptr;

	if (nextScene_) {
		delete nextScene_;
		nextScene_ = nullptr;
	}
}

void SceneManager::SetNextScene(BaseScene* nextScene) {
	if (!nextScene) { return; }
	nextScene_ = nextScene;
}

void SceneManager::SetNextScene(BaseScene* nextScene, const SceneParam& param) {
	if (!nextScene) { return; }
	nextScene->SetParam(param);
	nextScene_ = nextScene;
}
