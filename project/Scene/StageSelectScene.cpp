#include "StageSelectScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"

void StageSelectScene::Initialize() {
}

void StageSelectScene::Update(Input* input) {
	if (input->IsKeyTriggered(DIK_SPACE)) {
		GetSceneManager()->SetNextScene(new GamePlayScene());
	}
}

void StageSelectScene::Draw() {
	GetDx()->PreDraw();

	GetDx()->PostDraw();
}

void StageSelectScene::Finalize() {
}
