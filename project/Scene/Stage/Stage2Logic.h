#pragma once
#include "StageLogic.h"
#include "SoundManager.h"
#include <vector>

class Coin;
class Object3dManager;
class Player;

class Stage2Logic : public IStageLogic {
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
    std::vector<Coin*> coins_; // コイン配列

    // サウンド
    SoundManager* sound_ = nullptr;
    SoundData coin_ {};
};
