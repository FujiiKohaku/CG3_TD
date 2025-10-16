#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "StageSelectScene.h"
#include "DirectXCommon.h"

void TitleScene::Initialize() {
}

void TitleScene::Update(Input* input) {
	if (input->IsKeyTriggered(DIK_SPACE)) {
		GetSceneManager()->SetNextScene(new StageSelectScene());
	}
}

void TitleScene::Draw() {
	GetDx()->PreDraw();

	GetDx()->PostDraw();
}

void TitleScene::Finalize() {
}
