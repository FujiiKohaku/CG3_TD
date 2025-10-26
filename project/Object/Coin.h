#pragma once
#include "Object3d.h"
#include "Object3dManager.h"
#include "Struct.h"

class Coin {
public:
    void Initialize(Vector3 position, float scale, int score, Object3dManager* object3dManager, const std::string& modelName);
    void Update();
    void Draw();

    bool IsCollision(const Sphere& playerSphere);
    bool GetIsActive() const { return isActive_; }
    int GetScore() const { return score_; }

    ~Coin();

private:
    Vector3 position_ {};
    float radius_ = 1.0f;
    float scale_ = 1.0f;
    int score_ = 0;
    bool isActive_ = true; // 衝突したらfalseにする

    Object3d* object3d_ = nullptr;
    Sphere sphere_;
};
