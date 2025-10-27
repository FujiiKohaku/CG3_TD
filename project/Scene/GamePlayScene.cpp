#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "Input.h"
#include "Utility.h"
#include <filesystem>
#include <numbers>

// ---------------------------------------------
// コンストラクタ（ステージ番号受け取り）
// ---------------------------------------------
GamePlayScene::GamePlayScene(int stageNo)
{
    stageNo_ = stageNo;
}

// ---------------------------------------------
// 初期化処理
// ---------------------------------------------
void GamePlayScene::Initialize()
{

    // ---------------------------------------------
    // 例外処理・ログ
    // ---------------------------------------------
    SetUnhandledExceptionFilter(Utility::ExportDump);
    std::filesystem::create_directory("logs");

#pragma region 共通初期化

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

    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -100.0f });
    camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
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

#pragma endregion

    // =============================
    // ステージ別初期化
    // =============================
    switch (stageNo_) {
    case 1: {
        // 背景
        skydome_.Initialize(object3dManager_);

        // バンパー
        bumper_ = new Bumper();
        bumper_->Initialize({ 0.0f, 5.0f, 0.0f }, 5.0f, 1.2f, object3dManager_, "PlayerBall.obj");

        // ゴール
        goal_ = new Goal();
        goal_->Initialize({ 0.0f, 0.0f, 0.0f }, 3.0f, object3dManager_, "PlayerBall.obj");

        // プレイヤー
        pendulumPlayer_ = new Player();
        pendulumPlayer_->Initialize(1000, object3dManager_, "PlayerBall.obj");
        pendulumPlayer_->SetBumper(bumper_);
        pendulumPlayer_->SetGoal(goal_);

        // ワープゲート
        warpA_ = new WarpGate();
        warpA_->Initialize({ -10.0f, 3.0f, 0.0f }, 1.0f, object3dManager_, "plane.obj");
        warpB_ = new WarpGate();
        warpB_->Initialize({ 10.0f, 3.0f, 0.0f }, 1.0f, object3dManager_, "plane.obj");
        warpA_->SetPair(warpB_);
        warpB_->SetPair(warpA_);

        break;
    }

    case 2: { //ここからステージ2
        // 背景
        skydome_.Initialize(object3dManager_);


        // バンパー
        bumper_ = new Bumper();
        bumper_->Initialize({ 5.0f, 5.0f, 0.0f }, 5.0f, 1.2f, object3dManager_, "PlayerBall.obj");

        // コインを複数生成
        for (int i = 0; i < 5; i++) {
            Coin* coin = new Coin();
            coin->Initialize({ -5.0f + i * 3.0f, 3.0f, 0.0f }, 1.0f, 100, object3dManager_, "Coin.obj");
            coins_.push_back(coin);
        }

        // ゴール
        goal_ = new Goal();
        goal_->Initialize({ 15.0f, 0.0f, 0.0f }, 3.0f, object3dManager_, "PlayerBall.obj");

        // プレイヤー
        pendulumPlayer_ = new Player();
        pendulumPlayer_->Initialize(1000, object3dManager_, "PlayerBall.obj");
        pendulumPlayer_->SetBumper(bumper_);
  
        pendulumPlayer_->SetGoal(goal_);

        break;
    }

    default:
        skydome_.Initialize(object3dManager_);
        break;
    }

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

// ---------------------------------------------
// 更新処理
// ---------------------------------------------
void GamePlayScene::Update(Input* input)
{
    // ==============================
    // ImGui デバッグUI
    // ==============================
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Info");
    ImGui::Text("Stage: %d", stageNo_);
    if (pendulumPlayer_) {
        ImGui::Text("Score: %d", pendulumPlayer_->GetPoint());
    }
    ImGui::End();
    ImGui::Render();

    // ==============================
    // 更新処理
    // ==============================
    const BYTE* keys = input->GetKeys();
    const BYTE* preKeys = input->GetPreKeys();

    if (pendulumPlayer_) {
        pendulumPlayer_->Update(
            reinterpret_cast<const char*>(keys),
            reinterpret_cast<const char*>(preKeys),
            1.0f / 60.0f,
            input);
    }

    camera_->Update();
    skydome_.Update();

    // ==============================
    // ステージごとの個別処理
    // ==============================
    switch (stageNo_) {
    case 1: {
        // ステージ1：ワープゲート＋基本プレイ
        if (warpA_ && warpB_) {
            warpA_->Update();
            warpB_->Update();
            warpA_->CheckAndWarp(pendulumPlayer_);
            warpB_->CheckAndWarp(pendulumPlayer_);
        }
        break;
    }

    case 2: {
        // ステージ2：コイン・ブロックなどの処理
        
            // 必要ならブロックアニメーションや判定処理を追加
        

        if (!coins_.empty()) {
            Sphere playerSphere = { pendulumPlayer_->GetPosition(), pendulumPlayer_->GetRadius() };
            for (auto& coin : coins_) {
                coin->Update();
                if (coin->IsCollision(playerSphere)) {
                    pendulumPlayer_->AddScore(coin->GetScore());
                }
            }
        }
        break;
    }

    case 3: {
        // ステージ3：制限時間・特殊ギミックなど
        // （今後追加予定）
        break;
    }

    default:
        break;
    }

    // ==============================
    // ゴール処理
    // ==============================
    if (pendulumPlayer_ && pendulumPlayer_->GetIsGoal()) {
        GetSceneManager()->SetNextScene(new GameClearScene());
    }
}

// ---------------------------------------------
// 描画処理
// ---------------------------------------------
void GamePlayScene::Draw()
{
    GetDx()->PreDraw();
    object3dManager_->PreDraw();

    skydome_.Draw();

    switch (stageNo_) {
    // ===============================
    // ステージ1：ワープ＋バンパー構成
    // ===============================
    case 1: {
        if (bumper_)
            bumper_->Draw();
        if (goal_ && goal_->GetIsActive())
            goal_->Draw();
        if (pendulumPlayer_)
            pendulumPlayer_->Draw();
        if (warpA_)
            warpA_->Draw();
        if (warpB_)
            warpB_->Draw();
        break;
    }

    // ===============================
    // ステージ2：コイン・ブロック構成
    // ===============================
    case 2: {
        if (bumper_)
            bumper_->Draw();
        if (goal_ && goal_->GetIsActive())
            goal_->Draw();
        if (pendulumPlayer_)
            pendulumPlayer_->Draw();

        // コイン描画
        for (auto& coin : coins_) {
            coin->Draw();
        }
        break;
    }

    // ===============================
    // ステージ3：追加予定（敵やタイマーなど）
    // ===============================
    case 3: {
        if (pendulumPlayer_)
            pendulumPlayer_->Draw();
        if (goal_)
            goal_->Draw();
        // 敵やギミック追加予定
        break;
    }

    default:
        break;
    }

    // ===============================
    // ImGuiデバッグ表示（共通）
    // ===============================
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx()->GetCommandList());
    GetDx()->PostDraw();
}

// ---------------------------------------------
// 終了処理（解放）
// ---------------------------------------------
void GamePlayScene::Finalize()
{

    // 各オブジェクトを安全に削除
    delete pendulumPlayer_;
    pendulumPlayer_ = nullptr;
    delete bumper_;
    bumper_ = nullptr;
    delete goal_;
    goal_ = nullptr;
    delete warpA_;
    warpA_ = nullptr;
    delete warpB_;
    warpB_ = nullptr;
    delete spriteManager_;
    spriteManager_ = nullptr;
    delete object3dManager_;
    object3dManager_ = nullptr;

    sprites_.clear();

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
