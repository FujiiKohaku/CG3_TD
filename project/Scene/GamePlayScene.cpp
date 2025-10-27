#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "Input.h"
#include "Fade.h"
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
        bumper_ = new Bumper();
        bumper_->Initialize({ 5.0f, 5.0f, 0.0f }, 5.0f, 1.2f, object3dManager_, "PlayerBall.obj");

        // コインを縦×横に生成
        for (int y = 0; y < 10; y++) { // 縦方向（段）
            for (int x = 0; x < 10; x++) { // 横方向（列）
                Coin* coin = new Coin();

                Vector3 pos = { -6.0f + x * 3.0f, 3.0f + y * 2.5f, 0.0f };

                coin->Initialize(pos, 1.0f, 100, object3dManager_, "Coin.obj");
                coins_.push_back(coin);
            }
        }

        // ゴール
        goal_ = new Goal();
        goal_->Initialize({ 15.0f, 0.0f, 0.0f }, 3.0f, object3dManager_, "PlayerBall.obj");

        // プレイヤー
        pendulumPlayer_ = new Player();
        pendulumPlayer_->Initialize(1000, object3dManager_, "PlayerBall.obj");
        pendulumPlayer_->SetBumper(bumper_);

        pendulumPlayer_->SetGoal(goal_);


    // =============================
    // ステージ別初期化
    // =============================
   


	// フェードの初期化
	fade_ = new Fade();
	fade_->Initialize(GetDx());
	fade_->Start(Status::FadeIn, 0.25f);

	phase_ = Phase::kFadeIn;


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
	if (fade_) { fade_->Update(); }

	const BYTE* keys = nullptr;
	const BYTE* preKeys = nullptr;

	// ==============================
	//  フレームの先頭処理
	// ==============================
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (phase_ == Phase::kMain) {
		

		// ==============================
		//  開発用UI
		// ==============================

		// ImGui::ShowDemoWindow();
		// ==============================
		// ImGui デバッグ表示
		// ==============================
		ImGui::Begin("Player Debug");
		ImGui::Text("Score: %d", pendulumPlayer_->GetPoint()); // スコア表示

		ImGui::End();

		// ==============================
		//  更新処理（Update）
		// ==============================
		keys = input->GetKeys();
		preKeys = input->GetPreKeys();

		// 振り子プレイヤーの更新処理
		pendulumPlayer_->Update(reinterpret_cast<const char*>(keys), reinterpret_cast<const char*>(preKeys), 1.0f / 60.0f, input);

		// 各3Dオブジェクトの更新

		camera_->Update();
		skydome_.Update();
    
    // ステージ1：ワープゲート＋基本プレイ
        if (warpA_ && warpB_) {
            warpA_->Update();
            warpB_->Update();
            warpA_->CheckAndWarp(pendulumPlayer_);
            warpB_->CheckAndWarp(pendulumPlayer_);
        }
    // ステージ2：コイン・ブロックなどの処理

        // プレイヤーの球の情報を取得
        Sphere playerSphere = { pendulumPlayer_->GetPosition(), pendulumPlayer_->GetRadius() };

        // pendulum が「切れていない」＝まだ振り子で揺れている間はコインを取得できない
        if (pendulumPlayer_->GetPendulum()->GetIsCut()) {
            // 慣性移動中のみコイン判定を行う
            for (auto& coin : coins_) {
                coin->Update();
                if (coin->IsCollision(playerSphere)) {
                    pendulumPlayer_->AddScore(coin->GetScore());
                }
            }
        } else {
            // まだロープでつながっている間（スイング中）はコインだけ更新しておく
            for (auto& coin : coins_) {
                coin->Update();
            }
        }

		// プレイヤーがゴールしたらシーンを切り替える
		if (pendulumPlayer_->GetIsGoal() == true) {

			if (fade_) { fade_->Start(Status::FadeOut, 0.25f); }
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
			GetSceneManager()->SetNextScene(new GameClearScene());// クリアシーンができたらここに入れて
		}
		break;
	}

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
        pendulumPlayer_->Update(reinterpret_cast<const char*>(keys), reinterpret_cast<const char*>(preKeys), 1.0f / 60.0f, input);
    }

    camera_->Update();
    skydome_.Update();

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
	// ==============================
	//  描画処理（Draw）
	// ==============================

	// バックバッファの切り替え準備
	GetDx()->PreDraw();

	// ----- 3Dオブジェクト描画 -----
	object3dManager_->PreDraw(); // 3D描画準備
	skydome_.Draw();
	bumper_->Draw();
	block_->Draw();

	if (goal_->GetIsActive() == true) {
		goal_->Draw();
	}

	pendulumPlayer_->Draw();
	// ----- ImGui描画（デバッグUI） -----
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx()->GetCommandList());

	if (fade_) {
		fade_->Draw();
	}
  
  // ===============================
    // ImGuiデバッグ表示（共通）
    // ===============================
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx()->GetCommandList());

	// ----- 描画終了処理 -----
	GetDx()->PostDraw();

	// コマンドリスト状態確認ログ
	Logger::Log("CommandList state check before Close()");
}

// ---------------------------------------------
// 終了処理（解放）
// ---------------------------------------------
void GamePlayScene::Finalize()
{

    // 各オブジェクトを安全に削除
  delete fade_;
  fade_ = nullptr;
  delete goal_;
  goal_ = nullptr;
  delete block_;
  block_ = nullptr;
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
