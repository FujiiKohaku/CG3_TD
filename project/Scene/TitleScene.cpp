#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "TextureManager.h"
#include <numbers>
void TitleScene::Initialize()
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
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("titleTex.obj");
    ModelManager::GetInstance()->LoadModel("rainbow.obj");
    logoModel_ = ModelManager::GetInstance()->FindModel("titleTex.obj");
    BackModel_ = ModelManager::GetInstance()->FindModel("rainbow.obj");
    planeModel_ = ModelManager::GetInstance()->FindModel("plane.obj");
    //----------------------------------------
    //  Object3d 作成
    //----------------------------------------
    logoObject_ = new Object3d();
    logoObject_->Initialize(object3dManager_);
    logoObject_->SetModel(logoModel_);
    logoObject_->SetTranslate({ 0.0f, 0.0f, 0.0f });
    logoObject_->SetScale({ 1.0f, 1.0f, 1.0f });
    logoObject_->SetRotate({ std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float>, 0.0f });

    backGround_ = new Object3d();
    backGround_->Initialize(object3dManager_);
    backGround_->SetModel(BackModel_);
    backGround_->SetTranslate({ 0.0f, 0.0f, 5.0f });
    backGround_->SetScale({ 1.0f, 1.0f, 1.0f });
    backGround_->SetRotate({ 0.0f, std::numbers::pi_v<float>, 0.0f });

    plane_ = new Object3d();
    plane_->Initialize(object3dManager_);
    plane_->SetModel(planeModel_);
    plane_->SetTranslate({ 0.0f, 0.0f, 4.0f });
    plane_->SetScale({ 1.0f, 1.0f, 1.0f });
    plane_->SetRotate({ 0.0f, 0.0f, std::numbers::pi_v<float> / 2.0f });
}

void TitleScene::Update(Input* input)
{
    // スペースで次のシーンへ
    if (input->IsKeyTriggered(DIK_SPACE)) {
        GetSceneManager()->SetNextScene(new StageSelectScene());
    }

    logoObject_->Update();
    backGround_->Update();
    plane_->Update();
    camera_->Update();

    static float t = 0.0f;
    static float s = 0.0f;
    static float r = 0.0f;
    // 時間を進める
    t += 0.1f;
    s += 3.5f;
    r += 2.0f;
    // 爆発的拡大＋バウンド減衰
    float explosion = std::exp(-t * 1.0f) * std::sin(s * 100.0f); // 減衰振動
    float scale = 1.0f + std::abs(explosion) * 4.0f; // 最大約5倍！

    // 微振動で「ブルッ」と揺れる演出
    float shake = std::sin(s * 60.0f) * 0.05f;
    scale += shake;

    // スケール適用
    logoObject_->SetScale({ scale, scale, scale });

    // ループ
    if (t > 6.28f)
        t = 0.0f;
   // plane_->SetRotate({ 0.0f, r, 0.0f });
   // backGround_->SetRotate({ 0.0f, std::numbers::pi_v<float>, 0.0f });
}

void TitleScene::Draw()
{
    // 描画開始
    GetDx()->PreDraw();

    // 3D描画の準備
    object3dManager_->PreDraw();

    // モデル描画

    logoObject_->Draw();

    backGround_->Draw();

   // plane_->Draw();



    // 描画終了
    GetDx()->PostDraw();
}

void TitleScene::Finalize()
{
    delete backGround_;
    delete logoObject_;
    delete object3dManager_;
    delete camera_;

    ModelManager::GetInstance()->Finalize();
    TextureManager::GetInstance()->Finalize();
}
