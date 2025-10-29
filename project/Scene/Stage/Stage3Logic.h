#pragma once
#include "Stage/StageLogic.h"
#include <vector>

class Object3dManager;
class Player;
class Coin;

class Stage3Logic : public IStageLogic {
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
    std::vector<Coin*> coins_; // ƒRƒCƒ“”z—ñ
};