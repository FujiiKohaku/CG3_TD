#include "GamePlayScene.h"
#include "Input.h"
void GamePlayScene::Initialize()
{

    // 誰も補足しなかった場合(Unhandled),補足する関数を登録
    // main関数はじまってすぐに登録するとよい
    SetUnhandledExceptionFilter(Utility::ExportDump);
    // ログのディレクトリを用意
    std::filesystem::create_directory("logs");
    // main関数の先頭//

#pragma endregion

#pragma region object sprite

    // =============================
    // 2. テクスチャ・スプライト関係
    // =============================

    // TextureManager（シングルトン）
    TextureManager::GetInstance()->Initialize(GetDx());
    TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");

    // SpriteManager
    spriteManager_ = new SpriteManager();
    spriteManager_->Initialize(GetDx());

    // =============================
    // 3. 3D関連の初期化
    // =============================

    // Object3dManager
    object3dManager_ = new Object3dManager();
    object3dManager_->Initialize(GetDx());

    // カメラ
    camera_ = new Camera();
    camera_->SetTranslate({ 0.0f, 0.0f, -100.0f }); // 原点から離す
    camera_->SetRotate({ 0.0f, 0.0f, 0.0f }); // 下向き
    object3dManager_->SetDefaultCamera(camera_);
    // モデル共通設定

    ModelManager::GetInstance()->initialize(GetDx());
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("skydome.obj");
    ModelManager::GetInstance()->LoadModel("axis.obj");
    ModelManager::GetInstance()->LoadModel("PlayerBall.obj");
    ModelManager::GetInstance()->LoadModel("cube.obj");
    // =============================
    // 4. モデルと3Dオブジェクト生成
    // =============================

    //// モデルを生成
    // Model model; // 汎用モデル
    // model.Initialize(&modelCommon);

    // Model modelPlayer; // プレイヤー用モデル
    // modelPlayer.Initialize(&modelCommon);

    // Model modelEnemy; // 敵用モデル
    // modelEnemy.Initialize(&modelCommon);

    // 3Dオブジェクト生成

    object3d_.Initialize(object3dManager_);
    object3d_.SetModel("plane.obj");

    // プレイヤー

    // スカイドーム
    skydome_.Initialize(object3dManager_);

#pragma endregion

    //=================================
    // サウンドマネージャーインスタンス作成
    //=================================

    // サウンドマネージャー初期化！
    soundManager_.Initialize();
    // サウンドファイルを読み込み（パスはプロジェクトに合わせて調整）
    bgm = soundManager_.SoundLoadWave("Resources/BGM.wav");

    //=====================
    // バンパー
    //=====================
    bumper_ = new Bumper();
    bumper_->Initialize({ 0.0f, 5.0f, 0.0f }, 5.0f, 1.2f, object3dManager_, "PlayerBall.obj"); // 一時的にプレイヤーのモデルを入れてる

    scoreBumper_ = new ScoreBumper();
    scoreBumper_->Initialize({ 5.0f, 5.0f, 0.0f }, 0.5f, 1.2f, object3dManager_, "cube.obj"); // 一時的にcubeモデルを入れてる
    //=================================
    // 振り子プレイヤー
    //=================================
    pendulumPlayer_ = new Player();
    pendulumPlayer_->Initialize(object3dManager_, "PlayerBall.obj");
    pendulumPlayer_->SetBumper(bumper_);
    pendulumPlayer_->SetScoreBumper(scoreBumper_);
#ifdef _DEBUG

    Microsoft::WRL::ComPtr<ID3D12InfoQueue>
        infoQueue = nullptr;
    if (SUCCEEDED(GetDx()->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
        // やばいエラー時に止まる
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        // エラー時に止まる
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        // 警告時に止まる
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        // 抑制するメッセージのＩＤ
        D3D12_MESSAGE_ID denyIds[] = {
            // windows11でのDXGIデバックレイヤーとDX12デバックレイヤーの相互作用バグによるエラーメッセージ
            // https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
            D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
        };
        // 抑制するレベル
        D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
        D3D12_INFO_QUEUE_FILTER filter {};
        filter.DenyList.NumIDs = _countof(denyIds);
        filter.DenyList.pIDList = denyIds;
        filter.DenyList.NumSeverities = _countof(severities);
        filter.DenyList.pSeverityList = severities;
        // 指定したメッセージの表示wp抑制する
        infoQueue->PushStorageFilter(&filter);
        // 解放
        /*  infoQueue->Release();*/
    }
#endif // DEBUG
}

void GamePlayScene::Update(Input* input)
{
    // ==============================
    //  フレームの先頭処理
    // ==============================
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

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
    ImGui::Render(); // ImGuiの内部コマンドを生成（描画直前に呼ぶ）

    // ==============================
    //  更新処理（Update）
    // ==============================
    const BYTE* keys = input->GetKeys();
    const BYTE* preKeys = input->GetPreKeys();
    // 振り子プレイヤーの更新処理
    pendulumPlayer_->Update(reinterpret_cast<const char*>(keys), reinterpret_cast<const char*>(preKeys), 1.0f / 60.0f, input);

    // 各3Dオブジェクトの更新
    // object3d_.Update();
    // player2_.Update();
    // enemy_.Update();
    camera_->Update();
    skydome_.Update();

}

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
    scoreBumper_->Draw();
    // object3d_.Draw();
    // player2_.Draw();
    // enemy_.Draw();
    pendulumPlayer_->Draw();
    // ----- ImGui描画（デバッグUI） -----
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetDx()->GetCommandList());

    // ----- 描画終了処理 -----
    GetDx()->PostDraw();

    // コマンドリスト状態確認ログ
    Logger::Log("CommandList state check before Close()");
}

void GamePlayScene::Finalize()
{
    // 1. Player / Object3dなどユーザ作成オブジェクト破棄
    delete pendulumPlayer_;
    delete object3dManager_;
    delete spriteManager_;
    delete bumper_;
    delete scoreBumper_;
    sprites_.clear();

    // 2. ImGui破棄
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // 3. TextureManager / ModelManager
    ModelManager::GetInstance()->Finalize();
    TextureManager::GetInstance()->Finalize();

    // 4. Sound
    soundManager_.Finalize(&bgm);

    // 5. GPU待機
    GetDx()->WaitForGPU();
}
