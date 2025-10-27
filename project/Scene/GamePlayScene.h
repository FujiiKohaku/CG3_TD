#pragma once
// ======================= ヘッダー・ライブラリ関連 ==========================
#define _USE_MATH_DEFINES
// 標準ライブラリ//
#include "BaseScene.h"
#include "D3DResourceLeakChecker.h"
#include "DebugCamera.h"
#include "MatrixMath.h"
#include "ModelCommon.h"
#include "Object3D.h"
#include "Object3dManager.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Unknwn.h"
#include "Utility.h"
#include "modelManager.h"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <wrl.h>
// Windows・DirectX関連
#include <Windows.h> //ウィンドウAPIで消す
#include <d3d12.h>
#include <dbghelp.h>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
// 外部ライブラリ//
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/d3dx12.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
// リンカオプション
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")
// 個人ライブラリ
#include "Bumper.h"
#include "Player.h"
// #include "ScoreBumper.h"
#include "Coin.h"
#include "Skydome.h"
#include "WarpGate.h"
// シーン管理
#include "GameClearScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"

class Fade;

class GamePlayScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;
    GamePlayScene(int stageNo);

private:
    enum class Phase {
        kFadeIn,
        kMain,
        kFadeOut,
    };

    SoundManager soundManager_;
    Object3dManager* object3dManager_ = nullptr;
    SpriteManager* spriteManager_ = nullptr;
    DebugCamera debugCamera_;
    ModelCommon modelCommon_;
    std::vector<Sprite*> sprites_;
    Object3d object3d_, player2_, enemy_;
    SoundData bgm;
    bool isEnd = false;
    Player* pendulumPlayer_;
    Camera* camera_;
    Skydome skydome_;
    Bumper* bumper_;
    Goal* goal_;
    std::vector<Coin*> coins_; // コイン配列
    // ScoreBumper* scoreBumper_;

    Phase phase_ = Phase::kFadeIn;
    Fade* fade_ = nullptr;

    WarpGate* warpA_ = nullptr;
    WarpGate* warpB_ = nullptr;

private:
    int stageNo_ = 1; // 受け取ったステージ番号を保存
    // カメラ設定用の定数
    const float kCameraDistance = 100.0f; // プレイヤーからの距離
    const Vector3 kCameraInitPos = { 0.0f, 0.0f, -kCameraDistance };
    const Vector3 kCameraInitRot = { 0.0f, 0.0f, 0.0f };
    // バンパー設定用の定数
    const Vector3 kBumperPos = { 0.0f, 5.0f, 0.0f }; // バンパーの位置
    const float kBumperRadius = 5.0f; // バンパーの当たり判定半径
    const float kBumperBounce = 1.2f; // 反発係数
    // -------------------------------------
    // ゴール設定用の定数
    // -------------------------------------
    const Vector3 kGoalPosition = { 0.0f, 0.0f, 0.0f }; // ゴールの座標
    const float kGoalRadius = 3.0f; // ゴールの当たり判定（半径）
    // プレイヤー
    const int kClearPoint = 1000;
    // -------------------------------------
    // ワープゲート設定用の定数
    // -------------------------------------
    const Vector3 kWarpAPosition = { -10.0f, 3.0f, 0.0f }; // ワープAの位置
    const Vector3 kWarpBPosition = { 10.0f, 3.0f, 0.0f }; // ワープBの位置
    const float kWarpScale = 1.0f; // スケール
    const std::string kWarpModel = "plane.obj"; // 使用モデル名
};
