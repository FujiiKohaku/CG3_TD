#pragma once
struct Input;

class Object3dManager;
class Player;

class IStageLogic {
public:
    virtual ~IStageLogic() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;

    virtual void SetContext(Object3dManager* objMgr) { object3dManager_ = objMgr; }
    virtual void SetPlayer(Player* player) { pendulumPlayer_ = player; }

private:
    Object3dManager* object3dManager_ = nullptr;
    Player* pendulumPlayer_ = nullptr;
};
