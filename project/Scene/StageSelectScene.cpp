#include "StageSelectScene.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "Input.h"
#include "SceneManager.h"

void StageSelectScene::Initialize()
{
}

void StageSelectScene::Update(Input* input)
{

    if (input->IsKeyTriggered(DIK_LEFT)) {
        stageNo_--;
        if (stageNo_ < 1)
            stageNo_ = 3;
    }
    if (input->IsKeyTriggered(DIK_RIGHT)) {
        stageNo_++;
        if (stageNo_ > 3)
            stageNo_ = 1;
    }
    if (input->IsKeyTriggered(DIK_SPACE)) {
        GetSceneManager()->SetNextScene(new GamePlayScene(stageNo_));
    }
}

void StageSelectScene::Draw()
{
    GetDx()->PreDraw();

    GetDx()->PostDraw();
}

void StageSelectScene::Finalize()
{
}
