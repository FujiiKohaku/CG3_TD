#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "TextureManager.h"
class GameClearScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;

private:
    SpriteManager* spriteManager_ = nullptr;
    Object3dManager* object3dManager_ = nullptr;
    Object3d* logoObject_ = nullptr; // ロゴ3Dオブジェクト
    Model* logoModel_ = nullptr; // ロゴモデル
    Camera* camera_ = nullptr;
    Object3d* planeLine_ = nullptr; // 集中線
    Model* planeLineModel_ = nullptr;
};
