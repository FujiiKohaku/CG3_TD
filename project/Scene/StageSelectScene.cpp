#include "StageSelectScene.h"
#include "DirectXCommon.h"
#include "Fade.h"
#include "GamePlayScene.h"
#include "Input.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include "Stage/SceneParam.h"
#include <cmath>
#include <numbers>

void StageSelectScene::Initialize()
{
    fade_ = new Fade();
    fade_->Initialize(GetDx());
    fade_->Start(Status::FadeIn, 0.25f);

    // TextureManager::GetInstance()->Initialize(GetDx());

    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());

    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -25.0f });
    object3dManager_->SetDefaultCamera(camera_);

    ModelManager::GetInstance()->initialize(GetDx());
    ModelManager::GetInstance()->LoadModel("stage1.obj");
    ModelManager::GetInstance()->LoadModel("Allow.obj");
    // キューブ生成（3つ）
    for (int i = 0; i < 3; i++) {
        Object3d* cube = new Object3d();
        cube->Initialize(object3dManager_);
        cube->SetModel("stage1.obj");
        // cube->SetScale({ 1.0f, 1.0f, 1.0f });
        cube->SetScale({ 0.1f, 0.1f, 0.1f });
        cube->SetTranslate({ static_cast<float>(i * 6 - 6), 0.0f, 0.0f });
        cube->Update();
        cubes_.push_back(cube);
        scaleTimers_.push_back(0.0f);
    }

    allow_ = new Object3d();
    allow_->Initialize(object3dManager_);
    allow_->SetModel("Allow.obj");
    allow_->SetTranslate({ +6.0f, -2.0f, 0.0f });
    allow_->SetScale({ 1.0f, 1.0f, 1.0f });
    allow_->SetRotate({ 0.0f, std::numbers::pi_v<float> / 4.0f, 0.0f });

    allow2_ = new Object3d();
    allow2_->Initialize(object3dManager_);
    allow2_->SetModel("Allow.obj");
    allow2_->SetTranslate({ -6.0f, -2.0f, 0.0f }); // 位置も反対側に
    allow2_->SetScale({ 1.0f, 1.0f, 1.0f });
    allow2_->SetRotate({ 0.0f, -std::numbers::pi_v<float> / 2.0f, 0.0f });

    //----------------------------------------
    //  サウンド
    //----------------------------------------
    sound_ = new SoundManager();
    sound_->Initialize();
    selectSe_ = sound_->SoundLoadWave("resources/select.wav");
}

void StageSelectScene::Update(Input* input)
{
    if (fade_) {
        fade_->Update();
    }

    switch (phase_) {
    case Phase::kFadeIn:
        if (fade_->IsFinished()) {
            fade_->Stop();
            phase_ = Phase::kMain;
        }
        break;
    case Phase::kMain:
        if (input->IsKeyTriggered(DIK_LEFT) || input->IsTriggerLeftButton()) {
            selected_ = (selected_ - 1 + int(StageId::Count)) % int(StageId::Count);
        }

        if (input->IsKeyTriggered(DIK_RIGHT) || input->IsTriggerRightButton()) {
            selected_ = (selected_ + 1) % int(StageId::Count);
        }

        if (input->IsKeyTriggered(DIK_SPACE) || input->IsTriggerB()) {
            sound_->SoundPlayWave(selectSe_, false);
            fade_->Start(Status::FadeOut, 0.25f);
            phase_ = Phase::kFadeOut;
        }

        camera_->Update();

        // 各キューブ更新
        for (int i = 0; i < cubes_.size(); i++) {
            cubes_[i]->Update();
            allow_->Update();
            allow2_->Update();
            Vector3 rot = cubes_[i]->GetRotate();
            rot.y += 0.03f;
            cubes_[i]->SetRotate(rot);

            // --- イージング処理 ---
            if (i == selected_) {
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

        break;
    case Phase::kFadeOut:
        if (fade_->IsFinished()) {
            SceneParam param {};
            param.stage = static_cast<StageId>(selected_);
            GetSceneManager()->SetNextScene(new GamePlayScene(), param);
        }
        break;
    }
}

void StageSelectScene::Draw()
{
    GetDx()->PreDraw();

    object3dManager_->PreDraw();

    for (auto& cube : cubes_) {
        cube->Draw();
    }
    allow_->Draw();
    allow2_->Draw();
    fade_->Draw();

    GetDx()->PostDraw();
}

void StageSelectScene::Finalize()
{
    delete fade_;
    fade_ = nullptr;
    for (auto& cube : cubes_) {
        delete cube;
        cube = nullptr;
    }
    cubes_.clear();
    delete object3dManager_;
    object3dManager_ = nullptr;

    delete camera_;
    camera_ = nullptr;
    delete allow_;
    allow_ = nullptr;
    // ===== サウンド解放 =====
    if (sound_) {
        sound_->SoundUnload(&selectSe_);
        sound_->Finalize(nullptr);
        delete sound_;
        sound_ = nullptr;
    }
    ModelManager::GetInstance()->Finalize();
}
