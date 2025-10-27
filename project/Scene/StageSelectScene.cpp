#include "StageSelectScene.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"
#include "Input.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include <numbers>

// ---------------------------------------------
// 初期化
// ---------------------------------------------
void StageSelectScene::Initialize()
{
    // ==============================
    // TextureManager 初期化（必須）
    // ==============================
    TextureManager::GetInstance()->Initialize(GetDx());

    // ==============================
    // Object3dManager 初期化
    // ==============================
    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());

    // カメラ設定
    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -25.0f });
    object3dManager_->SetDefaultCamera(camera_);

    // モデル読み込み
    ModelManager::GetInstance()->initialize(GetDx());
    ModelManager::GetInstance()->LoadModel("cube.obj");

    // キューブ生成（3つ）
    for (int i = 0; i < 3; i++) {
        Object3d* cube = new Object3d();
        cube->Initialize(object3dManager_);
        cube->SetModel("cube.obj");
        cube->SetScale({ 1.0f, 1.0f, 1.0f });
        cube->SetTranslate({ static_cast<float>(i * 6 - 6), 0.0f, 0.0f }); // 見やすく、選択移動もわかりやすい

        cubes_.push_back(cube);
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

    // スペースキーで選択ステージへ
    if (input->IsKeyTriggered(DIK_SPACE)) {
        GetSceneManager()->SetNextScene(new GamePlayScene(stageNo_));
    }

    // カメラ更新
    camera_->Update();

    // 各キューブの見た目更新
    for (int i = 0; i < cubes_.size(); i++) {
        Vector3 rot = cubes_[i]->GetRotate();
        rot.y += 0.03f; // くるくる回転
        cubes_[i]->SetRotate(rot);

      // 選択中だけ大きく、それ以外は小さく
        Vector3 scale;
        if (i + 1 == stageNo_) {
            scale = { 2.0f, 2.0f, 2.0f }; // 選択中のステージ
        } else {
            scale = { 1.0f, 1.0f, 1.0f }; // 非選択ステージ
        }
        cubes_[i]->SetScale(scale);
    }
}

// ---------------------------------------------
// 描画
// ---------------------------------------------
void StageSelectScene::Draw()
{
    GetDx()->PreDraw();
    object3dManager_->PreDraw();

    // キューブを描画
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
