#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"

class TitleScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;

private:
    Object3d* logoObject_ = nullptr; // ロゴ3Dオブジェクト
    Model* logoModel_ = nullptr; // ロゴモデル
    Object3dManager* object3dManager_ = nullptr;
    Camera* camera_ = nullptr;
};