#pragma once
#include "StageLogic.h"

class Stage1Logic : public IStageLogic{
public:
    void Initialize() override;
    void Update(Input* input) override;
    void Draw() override;
    void Finalize() override;
private:
};

