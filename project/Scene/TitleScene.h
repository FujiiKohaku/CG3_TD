#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "SoundManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
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
    Object3d* logoObject_ = nullptr;
    Object3d* backGround_ = nullptr;
    Object3d* plane_ = nullptr;
    Object3d* giza_ = nullptr;
    Object3d* planeLine_ = nullptr;
    Object3d* spacelogo_ = nullptr;

    Model* logoModel_ = nullptr;
    Model* BackModel_ = nullptr;
    Model* planeModel_ = nullptr;
    Model* gizaModel_ = nullptr;
    Model* planeLineModel_ = nullptr;
    Model* spaceLogpModel_ = nullptr;

    Object3dManager* object3dManager_ = nullptr;
    Camera* camera_ = nullptr;
    Fade* fade_ = nullptr;

    Phase phase_ = Phase::kFadeIn;

    // 2D
    SpriteManager* spriteManager_ = nullptr;
    Sprite* sprite_ = nullptr;

    // サウンド
    SoundManager* sound_ = nullptr;
    SoundData selectSe_ {}; // 初期化しておく
};
