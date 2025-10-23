#include "GameClearScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include <numbers>

void GameClearScene::Initialize()
{
    //----------------------------------------
    //  TextureManager 初期化（最優先）
    //----------------------------------------
    TextureManager::GetInstance()->Initialize(GetDx());

    //----------------------------------------
    //  ModelManager 初期化
    //----------------------------------------
    ModelManager::GetInstance()->initialize(GetDx());

    //----------------------------------------
    // Object3dManager 初期化
    //----------------------------------------
    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());

    //----------------------------------------
    //  カメラ設定
    //----------------------------------------
    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
    camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
    object3dManager_->SetDefaultCamera(camera_);

    //----------------------------------------
    // モデル読み込み
    //----------------------------------------
    ModelManager::GetInstance()->LoadModel("clear.obj");
    ModelManager::GetInstance()->LoadModel("line.obj");
    logoModel_ = ModelManager::GetInstance()->FindModel("clear.obj");
    planeLineModel_ = ModelManager::GetInstance()->FindModel("line.obj");

    //----------------------------------------
    //  Object3d 作成
    //----------------------------------------
    logoObject_ = new Object3d();
    logoObject_->Initialize(object3dManager_);
    logoObject_->SetModel(logoModel_);
    logoObject_->SetTranslate({ -2.0f, 0.0f, 0.0f });
    logoObject_->SetScale({ 1.0f, 1.0f, 1.0f });
    logoObject_->SetRotate({ std::numbers::pi_v<float> / 2.0f, 0.0f, 0.0f });

    planeLine_ = new Object3d();
    planeLine_->Initialize(object3dManager_);
    planeLine_->SetModel(planeLineModel_);
    planeLine_->SetTranslate({ 0.0f, 0.0f, 3.0f });
    planeLine_->SetScale({ 5.0f, 5.0f, 5.0f });
    planeLine_->SetRotate({ 0.0f, -std::numbers::pi_v<float> / 4.0f, 0.0f });
}

void GameClearScene::Update(Input* input)
{
    // スペースキーで次のシーンへ
    if (input->IsKeyTriggered(DIK_SPACE)) {
        GetSceneManager()->SetNextScene(new StageSelectScene());
    }
    static float s = 0.0f;
    s += 3.5f;
    logoObject_->Update();
    camera_->Update();
    planeLine_->Update();
    planeLine_->SetRotate({ 0.0f, s, 0.0f });
}

void GameClearScene::Draw()
{
    // ===== 画面初期化 =====
    GetDx()->PreDraw();

    // ===== 3D描画 =====
    object3dManager_->PreDraw();
    logoObject_->Draw();
    planeLine_->Draw();
    // ===== 描画終了 =====
    GetDx()->PostDraw();
}

void GameClearScene::Finalize()
{

    delete logoObject_;
    delete object3dManager_;
    delete camera_;
    delete planeLine_;
    ModelManager::GetInstance()->Finalize();
    TextureManager::GetInstance()->Finalize();
}
