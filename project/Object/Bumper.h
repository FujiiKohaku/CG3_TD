#pragma once
#include "Struct.h"
#include "Object3d.h"
#include "Object3dManager.h"

class Bumper
{
    Vector3 position_ = {0.0f,0.0f,0.0f};
    float radius_ = 1.0f;
    float bounce_ = 1.0f;
    float scale_ = 1.0f;
    Object3d* object3d_ = nullptr;

    Sphere sphre_;

public:

    void Initialize(Vector3 position, float scale, float bounce, Object3dManager* object3dManager, const std::string& modelName);

    void Update();

    void Draw();

    float Length(const Vector3& v);

    // 球と球の衝突判定
    bool IsCollision(const Sphere& s1,const Sphere& s2);

    // 球と球の反射処理
    void ReflectSphereVelocity(Sphere& movingSphere, Vector3& velocity, const Sphere& staticSphere);

    // デストラクタ
    ~Bumper();

    // ゲッターロボ
    const Vector3& GetPosition() const { return position_; }
    const float& GetRadius() const { return radius_; }
    const float& GetBounce() const { return bounce_; }

    void SetPosition(Vector3 position) { position_ = position; }
    void SetBounce(float bounce) { bounce_ = bounce; }
    void SetRadius(float radius) { radius_ = radius; }

};

