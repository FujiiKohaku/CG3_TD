#include "GamePlayScene.h"
#include "Fade.h"
#include "GameClearScene.h"
#include "Input.h"
#include "Stage/StageFactory.h"
#include "Utility.h"
#include <filesystem>
#include <numbers>

void GamePlayScene::Initialize()
{

    // ---------------------------------------------
    // 例外処理・ログ
    // ---------------------------------------------
    SetUnhandledExceptionFilter(Utility::ExportDump);
    std::filesystem::create_directory("logs");

    // =============================
    // Texture / Sprite
    // =============================
    TextureManager::GetInstance()->Initialize(GetDx());
    TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");

    spriteManager_ = new SpriteManager();
    spriteManager_->Initialize(GetDx());

    // =============================
    // Object3D Manager / Camera
    // =============================
    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());
    // カメラ生成
    camera_ = new Camera();
    camera_->SetTranslate(kCameraInitPos);
    camera_->SetRotate(kCameraInitRot);
    object3dManager_->SetDefaultCamera(camera_);

    // =============================
    // モデル読み込み
    // =============================
    ModelManager::GetInstance()->initialize(GetDx());
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("skydome.obj");
    ModelManager::GetInstance()->LoadModel("axis.obj");
    ModelManager::GetInstance()->LoadModel("PlayerBall.obj");
    ModelManager::GetInstance()->LoadModel("cube.obj");
    ModelManager::GetInstance()->LoadModel("Coin.obj");

    // =============================
    // サウンド設定
    // =============================
    soundManager_.Initialize();
    bgm = soundManager_.SoundLoadWave("Resources/BGM.wav");

    // 背景
    skydome_.Initialize(object3dManager_);

    // バンパー生成
    bumper_ = new Bumper();
    bumper_->Initialize(kBumperPos, kBumperRadius, kBumperBounce, object3dManager_, "PlayerBall.obj");

    // ゴール
    goal_ = new Goal();
    goal_->Initialize(kGoalPosition, kGoalRadius, object3dManager_, "PlayerBall.obj");

    // プレイヤー
    pendulumPlayer_ = new Player();
    pendulumPlayer_->Initialize(kClearPoint, object3dManager_, "PlayerBall.obj");
    pendulumPlayer_->SetBumper(bumper_);
    pendulumPlayer_->SetGoal(goal_);

    // -------------------------------------
    // ワープゲート生成
    // -------------------------------------
    warpA_ = new WarpGate();
    warpA_->Initialize(kWarpAPosition, kWarpScale, object3dManager_, kWarpModel);

    warpB_ = new WarpGate();
    warpB_->Initialize(kWarpBPosition, kWarpScale, object3dManager_, kWarpModel);

    // お互いをリンク（ペア設定）
    warpA_->SetPair(warpB_);
    warpB_->SetPair(warpA_);

    // 背景
    skydome_.Initialize(object3dManager_);

    // バンパー
    //bumper_ = new Bumper();
    //bumper_->Initialize({ 5.0f, 5.0f, 0.0f }, 5.0f, 1.2f, object3dManager_, "PlayerBall.obj");

    // ゴール
    //goal_ = new Goal();
    //goal_->Initialize({ 15.0f, 0.0f, 0.0f }, 3.0f, object3dManager_, "PlayerBall.obj");

    // プレイヤー
    //pendulumPlayer_ = new Player();
    //pendulumPlayer_->Initialize(1000, object3dManager_, "PlayerBall.obj");
    //pendulumPlayer_->SetBumper(bumper_);

    //pendulumPlayer_->SetGoal(goal_);

    // フェードの初期化
    fade_ = new Fade();
    fade_->Initialize(GetDx());
    fade_->Start(Status::FadeIn, 0.25f);

    phase_ = Phase::kFadeIn;

    SceneParam param = GetParam();

    // stage_.reset();
    stage_ = CreateStageLogic(param.stage);
    stage_->SetContext(object3dManager_);
    stage_->SetPlayer(pendulumPlayer_);
    stage_->Initialize();

    scoreUI_ = new ScoreUI();
    scoreUI_->Initialize(spriteManager_);
#ifdef _DEBUG
    // GPUデバッグ設定
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
    if (SUCCEEDED(GetDx()->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        D3D12_MESSAGE_ID denyIds[] = {
            D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
        };
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_INFO_QUEUE_FILTER filter {};
        filter.DenyList.NumIDs = _countof(denyIds);
        filter.DenyList.pIDList = denyIds;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        infoQueue->PushStorageFilter(&filter);
    }
#endif
}

void GamePlayScene::Update(Input* input)
{
    if (fade_) {
        fade_->Update();
    }

    const BYTE* keys = nullptr;
    const BYTE* preKeys = nullptr;

    // ==============================
    //  フレームの先頭処理
    // ==============================
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
   
    if (phase_ == Phase::kMain) {
        int score = pendulumPlayer_->GetPoint(); // 例：プレイヤーのスコア取得
        scoreUI_->Update(score);
        // ==============================
        //  開発用UI
        // ==============================

        // ImGui::ShowDemoWindow();
        // ==============================
        // ImGui デバッグ表示
        // ==============================

        ImGui::Begin("Debug Info");
        if (pendulumPlayer_) {
            ImGui::Text("Score: %d", pendulumPlayer_->GetPoint());
        }
        ImGui::End();

        keys = input->GetKeys();
        preKeys = input->GetPreKeys();

        if (pendulumPlayer_) {
            pendulumPlayer_->Update(reinterpret_cast<const char*>(keys), reinterpret_cast<const char*>(preKeys), 1.0f / 60.0f, input);
        }

        camera_->Update();
        skydome_.Update();

        // ステージ1：ワープゲート＋基本プレイ
        if (warpA_ && warpB_) {
            warpA_->Update();
            warpB_->Update();
            warpA_->CheckAndWarp(pendulumPlayer_);
            warpB_->CheckAndWarp(pendulumPlayer_);
        }

        stage_->SetContext(object3dManager_);
        stage_->SetPlayer(pendulumPlayer_);
        stage_->Update();

        // プレイヤーがゴールしたらシーンを切り替える
        if (pendulumPlayer_->GetIsGoal()) {

            if (fade_) {
                fade_->Start(Status::FadeOut, 0.25f);
            }
            phase_ = Phase::kFadeOut;
        }
    }

    ImGui::Render(); // ImGuiの内部コマンドを生成（描画直前に呼ぶ）

    switch (phase_) {
    case Phase::kFadeIn:
        if (fade_ && fade_->IsFinished()) {
            fade_->Stop();
            phase_ = Phase::kMain;
        }
        break;

    case Phase::kFadeOut:
        if (fade_ && fade_->IsFinished()) {
            GetSceneManager()->SetNextScene(new GameClearScene()); // クリアシーンができたらここに入れて
        }
        break;
    }
}

void GamePlayScene::Draw()
{
    // ==============================
    // バックバッファの切り替え準備
    // ==============================
    GetDx()->PreDraw();

    // ==============================
    // 3D描画
    // ==============================
    object3dManager_->PreDraw();
    skydome_.Draw();
    bumper_->Draw();
    stage_->Draw();

    if (goal_->GetIsActive()) {
        goal_->Draw();
    }

    pendulumPlayer_->Draw();

    // ==============================
    // 2D描画（スプライト・UIなど）
    // ==============================
    spriteManager_->PreDraw(); // 2D描画の準備（PSO切り替え）
    scoreUI_->Draw(); // スコアUIを描画

    // フェードもSpriteを使っている場合はここ！
    if (fade_) {
        fade_->Draw();
    }

    // ==============================
    // ImGui描画
    // ==============================
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx()->GetCommandList());

    // ==============================
    // バックバッファ表示
    // ==============================
    GetDx()->PostDraw();

    Logger::Log("CommandList state check before Close()");
}

void GamePlayScene::Finalize()
{
    // 各オブジェクトを安全に削除
    delete camera_;
    camera_ = nullptr;
    delete fade_;
    fade_ = nullptr;
    delete goal_;
    goal_ = nullptr;
    delete pendulumPlayer_;
    pendulumPlayer_ = nullptr;
    delete bumper_;
    bumper_ = nullptr;
    delete warpA_;
    warpA_ = nullptr;
    delete warpB_;
    warpB_ = nullptr;
    delete spriteManager_;
    spriteManager_ = nullptr;
    delete object3dManager_;
    object3dManager_ = nullptr;
    delete scoreUI_;
    scoreUI_ = nullptr;
    sprites_.clear();

    if (stage_) {
        stage_->Finalize();
        //delete stage_;
        stage_ = nullptr;
    }

    // ImGui破棄
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // マネージャ系解放
    ModelManager::GetInstance()->Finalize();
    TextureManager::GetInstance()->Finalize();

    // サウンド解放
    soundManager_.Finalize(&bgm);

    // GPU待機
    GetDx()->WaitForGPU();
}
