#include "ScoreBumper.h"

void ScoreBumper::Initialize(Vector3 position, float radius, float bounce, Object3dManager* object3dManager, const std::string& modelName)
{
    position_ = position;
    radius_ = radius;
    bounce_ = bounce;
    object3d_ = new Object3d;
    object3d_->Initialize(object3dManager);
    object3d_->SetModel(modelName);
}

void ScoreBumper::Update()
{
    sphre_ = { position_, radius_ };
}

void ScoreBumper::Draw()
{
    // 現在の位置に合わせる
    object3d_->SetTranslate(position_);
    object3d_->Update();
    object3d_->Draw();
}

float ScoreBumper::Length(const Vector3& v)
{
    float length;

    length = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));

    return length;
}

bool ScoreBumper::IsCollision(const Sphere& s1, const Sphere& s2)
{
    // 中心間ベクトルを計算
    Vector3 delta = {
        s2.center.x - s1.center.x,
        s2.center.y - s1.center.y,
        s2.center.z - s1.center.z
    };

    // ２つの中心間距離
    float distance = Length(delta);

    if (distance <= s1.radius + s2.radius) {
        return true;

    } else {
        return false;
    }
}

void ScoreBumper::ReflectSphereVelocity(Sphere& movingSphere, Vector3& velocity, const Sphere& staticSphere)
{
    Vector3 diff = {
        movingSphere.center.x - staticSphere.center.x,
        movingSphere.center.y - staticSphere.center.y,
        movingSphere.center.z - staticSphere.center.z
    };

    float dist = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    float sumRadius = movingSphere.radius + staticSphere.radius;

    if (dist < sumRadius && dist > 1e-6f) {
        // 正規化法線
        Vector3 n = { diff.x / dist, diff.y / dist, diff.z / dist };

        // めり込み補正
        float penetration = sumRadius - dist;
        movingSphere.center.x += n.x * penetration;
        movingSphere.center.y += n.y * penetration;
        movingSphere.center.z += n.z * penetration;

        // 反射処理（入射角＝反射角）
        float dotN = velocity.x * n.x + velocity.y * n.y + velocity.z * n.z;
        velocity.x -= 2.0f * dotN * n.x;
        velocity.y -= 2.0f * dotN * n.y;
        velocity.z -= 2.0f * dotN * n.z;

        // 反射倍率
        velocity.x *= bounce_;
        velocity.y *= bounce_;
        velocity.z *= bounce_;
    }
}

ScoreBumper::~ScoreBumper()
{
    if (object3d_) {
        delete object3d_;
        object3d_ = nullptr;
    }
}
