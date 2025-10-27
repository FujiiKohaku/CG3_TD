#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include <vector>

/// <summary>
/// ステージセレクトシーン
/// 3つのcubeでステージ選択を行う
/// ←→で選択、スペースで決定
/// </summary>
class StageSelectScene : public BaseScene {
public:
    void Initialize() override; // 初期化
    void Update(Input* input) override; // 更新処理
    void Draw() override; // 描画処理
    void Finalize() override; // 終了処理

private:
    int stageNo_ = 1; // 現在選択中のステージ番号（1〜3）

    // === 3D関連 ===
    Object3dManager* object3dManager_ = nullptr; // 3Dマネージャ
    Camera* camera_ = nullptr; // カメラ
    std::vector<Object3d*> cubes_; // ステージ選択用の立方体
};
