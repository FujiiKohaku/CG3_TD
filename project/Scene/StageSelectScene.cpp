#include "StageSelectScene.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "Input.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include <cmath>
#include <numbers>

// ---------------------------------------------
// 初期化
// ---------------------------------------------
void StageSelectScene::Initialize()
{
    TextureManager::GetInstance()->Initialize(GetDx());

    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());

    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -25.0f });
    object3dManager_->SetDefaultCamera(camera_);

    ModelManager::GetInstance()->initialize(GetDx());
    ModelManager::GetInstance()->LoadModel("cube.obj");

    // キューブ生成（3つ）
    for (int i = 0; i < 3; i++) {
        Object3d* cube = new Object3d();
        cube->Initialize(object3dManager_);
        cube->SetModel("cube.obj");
        cube->SetScale({ 1.0f, 1.0f, 1.0f });
        cube->SetTranslate({ static_cast<float>(i * 6 - 6), 0.0f, 0.0f });

        cubes_.push_back(cube);
        scaleTimers_.push_back(0.0f); // ← イージング用タイマー追加
    }
}

// ---------------------------------------------
// 更新
// ---------------------------------------------
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

    camera_->Update();

    // 各キューブ更新
    for (int i = 0; i < cubes_.size(); i++) {
        Vector3 rot = cubes_[i]->GetRotate();
        rot.y += 0.03f;
        cubes_[i]->SetRotate(rot);

        // --- イージング処理 ---
        if (i + 1 == stageNo_) {
            // 選択中 → タイマー進行
            scaleTimers_[i] += 0.05f;
            if (scaleTimers_[i] > 1.0f)
                scaleTimers_[i] = 1.0f;
        } else {
            // 非選択 → タイマー減衰
            scaleTimers_[i] -= 0.05f;
            if (scaleTimers_[i] < 0.0f)
                scaleTimers_[i] = 0.0f;
        }

        // --- イージング関数（sin波でふわっと） ---
        float ease = (std::sin(scaleTimers_[i] * std::numbers::pi_v<float> / 2.0f));

        // --- スケール反映 ---
        float baseScale = 1.0f;
        float maxScale = 2.0f;
        float scaleValue = baseScale + (maxScale - baseScale) * ease;
        cubes_[i]->SetScale({ scaleValue, scaleValue, scaleValue });
    }
}

// ---------------------------------------------
// 描画
// ---------------------------------------------
void StageSelectScene::Draw()
{
    GetDx()->PreDraw();
    object3dManager_->PreDraw();

    for (auto& cube : cubes_) {
        cube->Update();
        cube->Draw();
    }

    GetDx()->PostDraw();
}

// ---------------------------------------------
// 終了処理
// ---------------------------------------------
void StageSelectScene::Finalize()
{
    for (auto& cube : cubes_) {
        delete cube;
    }
    cubes_.clear();

    delete object3dManager_;
    object3dManager_ = nullptr;

    delete camera_;
    camera_ = nullptr;

    ModelManager::GetInstance()->Finalize();
}
