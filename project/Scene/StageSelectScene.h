#pragma once
#include "BaseScene.h"

class StageSelectScene : public BaseScene {
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;

private:
    int stageNo_ = 1;
};
