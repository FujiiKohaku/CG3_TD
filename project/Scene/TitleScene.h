#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "Sprite.h"
#include "SpriteManager.h"

class TitleScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;

private:
    Object3d* logoObject_ = nullptr; // ロゴ3Dオブジェクト
    Object3d* backGround_ = nullptr; // 背景
    Object3d* plane_ = nullptr; // 背景
    Object3d* giza_ = nullptr;
    Object3d* planeLine_ = nullptr; // 集中線
    Model* logoModel_ = nullptr; // ロゴモデル
    Model* BackModel_ = nullptr;
    Model* planeModel_ = nullptr;
    Model* gizaModel_ = nullptr;
    Model* planeLineModel_ = nullptr;
    Object3dManager* object3dManager_ = nullptr;
    Camera* camera_ = nullptr;
    // スプライト
    SpriteManager* spriteManager_ = nullptr;
    Sprite* sprite_ = nullptr;
};