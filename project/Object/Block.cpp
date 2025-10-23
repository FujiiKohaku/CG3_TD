#include "Block.h"

void Block::Initialize(Vector3 position, float width, float height,float bounce, Object3dManager* object3dManager, const std::string& modelName)
{
	centerPosition_ = position;
	width_ = width;
	height_ = height;
	scaleX_ = width_/2.0f;
	scaleY_ = height_/2.0f;
	bounce_ = bounce;

    block_ = {
        {centerPosition_.x - width_ / 2.0f,centerPosition_.y - height_ / 2.0f,-0.5f},
        {centerPosition_.x + width_ / 2.0f,centerPosition_.y + height_ / 2.0f,+0.5f}
    };

    object3d_ = new Object3d;
    object3d_->Initialize(object3dManager);
    object3d_->SetModel(modelName);

}

void Block::Draw()
{
    // 現在の位置に合わせる
    object3d_->SetTranslate(centerPosition_);
    object3d_->Update();
    object3d_->Draw();
    object3d_->SetScale({ scaleX_, scaleY_, scaleZ_ });
}

Vector3 Block::Add(const Vector3& v1, const Vector3& v2)
{
    Vector3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

Vector3 Block::Multiply(const float& i, const Vector3& v1)
{
    Vector3 result;

    result.x = v1.x * i;
    result.y = v1.y * i;
    result.z = v1.z * i;

    return result;
}

Vector3 Block::Subtract(const Vector3& v1, const Vector3& v2)
{
    Vector3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

float Block::Dot(const Vector3& v1, const Vector3& v2)
{
    float resoult;

    resoult = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    return resoult;
}

float Block::Length(const Vector3& v)
{
    float length;

    length = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));

    return length;
}

bool Block::IsCollision(const AABB& aabb, const Sphere& spher)
{
    Vector3 clossPoint{
        std::clamp(spher.center.x, aabb.min.x, aabb.max.x),
        std::clamp(spher.center.y, aabb.min.y, aabb.max.y),
        std::clamp(spher.center.z, aabb.min.z, aabb.max.z),
    };

    float distance = Length(Subtract(clossPoint, spher.center));

    if (distance <= spher.radius) {
        return true;

    } else {
        return false;
    }
}

Block::~Block()
{
    if (object3d_) {
        delete object3d_;
        object3d_ = nullptr;
    }
}

void Block::ReflectSphereFromAABB(Vector3& position, Vector3& velocity, const AABB& aabb, float radius, float bounce)
{
    // 衝突点に最も近い点を求める
    Vector3 closest{
        std::clamp(position.x, aabb.min.x, aabb.max.x),
        std::clamp(position.y, aabb.min.y, aabb.max.y),
        std::clamp(position.z, aabb.min.z, aabb.max.z)
    };

    // AABB の面ごとの法線を判定
    Vector3 normal = { 0, 0, 0 };
    float pxMin = fabsf((position.x + radius) - aabb.min.x);
    float pxMax = fabsf((position.x - radius) - aabb.max.x);
    float pyMin = fabsf((position.y + radius) - aabb.min.y);
    float pyMax = fabsf((position.y - radius) - aabb.max.y);

    float minPenetration = FLT_MAX;

    if (pxMin < minPenetration && position.x < aabb.min.x) {
        normal = { -1, 0, 0 };
        minPenetration = pxMin;
    }
    if (pxMax < minPenetration && position.x > aabb.max.x) {
        normal = { 1, 0, 0 };
        minPenetration = pxMax;
    }
    if (pyMin < minPenetration && position.y < aabb.min.y) {
        normal = { 0, -1, 0 };
        minPenetration = pyMin;
    }
    if (pyMax < minPenetration && position.y > aabb.max.y) {
        normal = { 0, 1, 0 };
        minPenetration = pyMax;
    }

    // めり込み補正
    position = Add(position, Multiply(minPenetration * normal.x + minPenetration * normal.y, normal));

    // 反射処理（R = V - 2*(V・N)*N）
    float dotN = Dot(velocity, normal);
    velocity = Subtract(velocity, Multiply(2.0f * dotN, normal));

    // 反射倍率
    velocity = Multiply(bounce, velocity);
}
