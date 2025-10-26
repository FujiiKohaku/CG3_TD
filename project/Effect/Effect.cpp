#include "Effect.h"
#include <cmath>
#include <cstdlib> // rand()

void Effect::Initialize(Object3dManager* object3dManager, const std::string& modelName)
{
    object3dManager_ = object3dManager;
    modelName_ = modelName;

    // 粒を初期化
    for (int i = 0; i < kMaxParticles; i++) {
        particles_[i].object = new Object3d();
        particles_[i].object->Initialize(object3dManager_);
        particles_[i].object->SetModel(modelName_);
        particles_[i].object->SetScale({ 0.2f, 0.2f, 0.2f });
        particles_[i].life = 0; // 最初は非アクティブ
    }
}

void Effect::Emit(const Vector3& position)
{
    for (int i = 0; i < kMaxParticles; i++) {
        particles_[i].object->SetTranslate(position);
        particles_[i].object->SetScale({ 0.2f, 0.2f, 0.2f });

        // ランダムに散らばる方向
        particles_[i].velocity = {
            ((float)rand() / RAND_MAX - 0.5f) * 0.8f,
            ((float)rand() / RAND_MAX) * 0.8f,
            ((float)rand() / RAND_MAX - 0.5f) * 0.8f
        };

        particles_[i].life = 20 + rand() % 10; // 寿命を少しランダムに
    }
}

void Effect::Update()
{
    for (int i = 0; i < kMaxParticles; i++) {
        if (particles_[i].life <= 0)
            continue;

        particles_[i].life--;

        // 重力と減速
        particles_[i].velocity.y -= 0.02f;
        particles_[i].velocity.x *= 0.97f;
        particles_[i].velocity.z *= 0.97f;

        // 移動
        Vector3 pos = particles_[i].object->GetTranslate();
        pos = pos + particles_[i].velocity;
        particles_[i].object->SetTranslate(pos);

        // 徐々に小さく
        Vector3 scale = particles_[i].object->GetScale();
        scale = scale * 0.95f;
        particles_[i].object->SetScale(scale);

        particles_[i].object->Update();
    }
}

void Effect::Draw()
{
    for (int i = 0; i < kMaxParticles; i++) {
        if (particles_[i].life > 0) {
            particles_[i].object->Draw();
        }
    }
}

void Effect::Finalize()
{
    for (int i = 0; i < kMaxParticles; i++) {
        delete particles_[i].object;
        particles_[i].object = nullptr;
    }
}
