#pragma once
#include "Object3d.h"
#include "Object3dManager.h"
#include <string>

class Effect {
private:
    static const int kMaxParticles = 10; // 同時に出す数
    struct Particle {
        Object3d* object = nullptr;
        Vector3 velocity;
        int life = 0;
    };

    Object3dManager* object3dManager_ = nullptr;
    Particle particles_[kMaxParticles];
    std::string modelName_; // モデル名を外部指定可能に

public:
    void Initialize(Object3dManager* object3dManager, const std::string& modelName);
    void Emit(const Vector3& position);
    void Update();
    void Draw();
    void Finalize();
};
