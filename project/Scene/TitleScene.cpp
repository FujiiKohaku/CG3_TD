#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Fade.h"
#include "Input.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
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
    //  2D
    //----------------------------------------

    // テクスチャロード
    TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
    spriteManager_ = new SpriteManager();
    spriteManager_->Initialize(GetDx());
    // スプライト生成
    // sprite_ = new Sprite();
    // sprite_->Initialize(spriteManager_, "resources/uvChecker.png");
    // 位置・サイズ・色設定
    // sprite_->SetPosition({ 640.0f, 360.0f }); // 画面中央
    // sprite_->SetSize({ 100.0f, 100.0f }); // 大きめに

    //----------------------------------------
    // モデル読み込み
    //----------------------------------------
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("titleTex.obj");
    ModelManager::GetInstance()->LoadModel("rainbow.obj");
    ModelManager::GetInstance()->LoadModel("giza.obj");
    ModelManager::GetInstance()->LoadModel("line.obj");
    ModelManager::GetInstance()->LoadModel("PushSpace.obj");
    logoModel_ = ModelManager::GetInstance()->FindModel("titleTex.obj");
    BackModel_ = ModelManager::GetInstance()->FindModel("rainbow.obj");
    planeModel_ = ModelManager::GetInstance()->FindModel("plane.obj");
    gizaModel_ = ModelManager::GetInstance()->FindModel("giza.obj");
    planeLineModel_ = ModelManager::GetInstance()->FindModel("line.obj");
    spaceLogpModel_ = ModelManager::GetInstance()->FindModel("PushSpace.obj");
    //----------------------------------------
    //  Object3d 作成
    //----------------------------------------
    logoObject_ = new Object3d();
    logoObject_->Initialize(object3dManager_);
    logoObject_->SetModel(logoModel_);
    logoObject_->SetTranslate({ 0.0f, 0.0f, 0.0f });
    logoObject_->SetScale({ 1.0f, 1.0f, 1.0f });
    logoObject_->SetRotate({ std::numbers::pi_v<float> / 2.0f, 0.0f, 0.0f });

    backGround_ = new Object3d();
    backGround_->Initialize(object3dManager_);
    backGround_->SetModel(BackModel_);
    backGround_->SetTranslate({ 0.0f, 0.0f, 5.0f });
    backGround_->SetScale({ 8.0f, 4.0f, 1.0f });
    backGround_->SetRotate({ 0.0f, 0.0f, 0.0f });

    plane_ = new Object3d();
    plane_->Initialize(object3dManager_);
    plane_->SetModel(planeModel_);
    plane_->SetTranslate({ 0.0f, 0.0f, 4.0f });
    plane_->SetScale({ 1.0f, 1.0f, 1.0f });
    plane_->SetRotate({ 0.0f, 0.0f, std::numbers::pi_v<float> / 2.0f });

    fade_ = new Fade();
    fade_->Initialize(GetDx());
    fade_->Start(Status::FadeIn, 0.25f);

    giza_ = new Object3d();
    giza_->Initialize(object3dManager_);
    giza_->SetModel(gizaModel_);
    giza_->SetTranslate({ 0.0f, 0.0f, 4.0f });
    giza_->SetScale({ 1.0f, 1.0f, 1.0f });
    giza_->SetRotate({ std::numbers::pi_v<float> / 2.0f, 0.0f, 0.0f });

    planeLine_ = new Object3d();
    planeLine_->Initialize(object3dManager_);
    planeLine_->SetModel(planeLineModel_);
    planeLine_->SetTranslate({ 0.0f, 0.0f, 3.0f });
    planeLine_->SetScale({ 5.0f, 5.0f, 5.0f });
    planeLine_->SetRotate({ 0.0f, -std::numbers::pi_v<float> / 4.0f, 0.0f });

    spacelogo_ = new Object3d();
    spacelogo_->Initialize(object3dManager_);
    spacelogo_->SetModel(spaceLogpModel_);
    spacelogo_->SetTranslate({ -2.0f, -1.0f, 0.0f });
    spacelogo_->SetScale({ 0.5f, 0.5f, 0.5f });
    spacelogo_->SetRotate({ std::numbers::pi_v<float> / 2.0f, 0.0f, 0.0f });
}

void TitleScene::Update(Input* input)
{
    fade_->Update();

    switch (phase_) {
    case Phase::kFadeIn:
        if (fade_->IsFinished()) {
            fade_->Stop();
            phase_ = Phase::kMain;
        }
        break;
    case Phase::kMain:
        if (input->IsKeyTriggered(DIK_SPACE) || input->IsTriggerB()) {
            // GetSceneManager()->SetNextScene(new StageSelectScene());
            fade_->Start(Status::FadeOut, 0.25f);
            phase_ = Phase::kFadeOut;
        }
        break;
    case Phase::kFadeOut:
        // フェードアウト処理
        if (fade_->IsFinished()) {
            // シーン切り替え
            GetSceneManager()->SetNextScene(new StageSelectScene());
        }
        break;
    }

    logoObject_->Update();
    backGround_->Update();
    plane_->Update();
    camera_->Update();
    giza_->Update();
    planeLine_->Update();
    spacelogo_->Update();
    // planeLine_->Update();
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

    float ease = (std::sin(t) + 1.0f) / 2.0f;
    float rotateY = std::sin(t * 0.8f) * 0.5f;
    float S = 1.0f + ease * 0.3f;

    giza_->SetRotate({ t, rotateY, t });
    giza_->SetScale({ S, S, S });
    planeLine_->SetRotate({ 0.0f, s, 0.0f });
}

void TitleScene::Draw()
{
    // ===== 画面初期化 =====
    GetDx()->PreDraw();

    // ===== 3D描画 =====
    object3dManager_->PreDraw();
    logoObject_->Draw();
    spacelogo_->Draw();
    giza_->Draw();
    backGround_->Draw();
    planeLine_->Draw();
    // ===== 2D描画（最後に）=====
    spriteManager_->PreDraw(); // ← PSO切り替え

    fade_->Draw();

    // ===== 描画終了 =====
    GetDx()->PostDraw();
}

void TitleScene::Finalize()
{
    delete plane_;
    delete backGround_;
    delete logoObject_;
    delete object3dManager_;
    delete camera_;
    delete giza_;
    delete planeLine_;
    delete sprite_;
    delete spriteManager_;
    delete spacelogo_;
    ModelManager::GetInstance()->Finalize();
    TextureManager::GetInstance()->Finalize();
}
