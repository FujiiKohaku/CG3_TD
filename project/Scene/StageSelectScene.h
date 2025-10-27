#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include <vector>

class StageSelectScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;

private:
    int stageNo_ = 1;

    Object3dManager* object3dManager_ = nullptr;
    Camera* camera_ = nullptr;
    std::vector<Object3d*> cubes_;
    std::vector<float> scaleTimers_; // ← イージング用タイマー
};
