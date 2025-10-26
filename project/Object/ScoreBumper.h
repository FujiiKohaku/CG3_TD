#pragma once
#include "Object3d.h"
#include "Object3dManager.h"
#include "Struct.h"

class ScoreBumper {
public:
    ~ScoreBumper();
    Vector3 position_ = { 0.0f, 0.0f, 0.0f };
    float radius_ = 0.0f;
    float bounce_ = 1.0f;
    Object3d* object3d_ = nullptr;

    Sphere sphre_;

public:
    void Initialize(Vector3 position, float radius, float bounce, Object3dManager* object3dManager, const std::string& modelName);

    void Update();

    void Draw();

    float Length(const Vector3& v);

    // 球と球の衝突判定
    bool IsCollision(const Sphere& s1, const Sphere& s2);

    // 球と球の反射処理
    void ReflectSphereVelocity(Sphere& movingSphere, Vector3& velocity, const Sphere& staticSphere);

    // ゲッターロボ
    const Vector3& GetPosition() const { return position_; }
    const float& GetRadius() const { return radius_; }
    const float& GetBounce() const { return bounce_; }

    void SetPosition(Vector3 position) { position_ = position; }
    void SetPosition(float bounce) { bounce_ = bounce; }
    void SetRadius(float radius) { radius_ = radius; }
};
