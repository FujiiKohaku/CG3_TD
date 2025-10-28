#pragma once
#include "StageLogic.h"
#include "Struct.h"
#include <iostream>

class WarpGate;

class Stage1Logic : public IStageLogic{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

    void SetContext(Object3dManager* objMgr) override { object3dManager_ = objMgr; }
    void SetPlayer(Player* player) override { pendulumPlayer_ = player; }

private:
    Object3dManager* object3dManager_ = nullptr;
    Player* pendulumPlayer_ = nullptr;

    WarpGate* warpA_ = nullptr;
    WarpGate* warpB_ = nullptr;

    // -------------------------------------
    // ワープゲート設定用の定数
    // -------------------------------------
    const Vector3 kWarpAPosition = { -10.0f, 3.0f, 0.0f }; // ワープAの位置
    const Vector3 kWarpBPosition = { 10.0f, 3.0f, 0.0f }; // ワープBの位置
    const float kWarpScale = 1.0f; // スケール
    const std::string kWarpModel = "plane.obj"; // 使用モデル名
};

