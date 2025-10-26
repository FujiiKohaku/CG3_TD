#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"

class Fade;

enum class Phase {
    kFadeIn,
    kMain,
    kFadeOut,
};

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
    Model* logoModel_ = nullptr; // ロゴモデル
    Model* BackModel_ = nullptr;
    Model* planeModel_ = nullptr;
    Object3dManager* object3dManager_ = nullptr;
    Camera* camera_ = nullptr;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};