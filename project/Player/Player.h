#pragma once
#include "Input.h"
#include "MatrixMath.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "Pendulum.h"
#include "Struct.h"
#include "algorithm"
#include "cmath"

class Player {

    Vector3 position_ = { 0.0f, 0.2f, 0.0f };
    Vector3 prevPosition_ = { 0.0f, 0.0f, 0.0f };
    Vector3 anchorPosition_ = { 0.0f, 0.0f, 0.0f };
    Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
    float decelerationRate_ = 0.98f;
    float mass_ = 2.0f;
    float radius_ = 0.05f;
    unsigned int color_ = 0xFF0000FF;

    Pendulum* pendulum_ = nullptr;
    Object3d* object3d_ = nullptr;
    // 壁の位置の初期化ほんとはここに入れるのは良くない
    const float wallXMin = -7.5f;
    const float wallXMax = 7.5f;
    const float wallYMin = -1.0f;
    const float wallYMax = 14.0f;

    // 壁衝突処理（カプセル判定）
    Vector3 walls_[4][2] = {
        { { wallXMin, wallYMin, 0 }, { wallXMin, wallYMax, 0 } },
        { { wallXMax, wallYMin, 0 }, { wallXMax, wallYMax, 0 } },
        { { wallXMin, wallYMin, 0 }, { wallXMax, wallYMin, 0 } },
        { { wallXMin, wallYMax, 0 }, { wallXMax, wallYMax, 0 } }
    };

    Vector3 bumperPos_ = { 0.0f, 5.0f, 0.0f };
    float bumperRadius_ = 0.5f;
    float bounce_ = 1.2f;

public:
    ~Player();
    // 内積
    float Dot(const Vector3& v1, const Vector3& v2);

    Vector3 Add(const Vector3& v1, const Vector3& v2);

    Vector3 Subtract(const Vector3& v1, const Vector3& v2);

    Vector3 Multiply(const float& i, const Vector3& v1);

    Matrix4x4 Add(Matrix4x4 matrix1, Matrix4x4 matrix2);

    Matrix4x4 Subtract(Matrix4x4 matrix1, Matrix4x4 matrix2);

    float LengthSq(const Vector3& v);

    float Length(const Vector3& v);

    bool CapsuleIntersectsSegment3D(const Vector3& capsuleStart, const Vector3& capsuleEnd, float radius, const Vector3& segStart, const Vector3& segEnd);

    // --- 壁外チェック（OB処理） ---
    bool isOutOfBounds();

    // 球と球の衝突判定
    bool SphereIntersectsSphere(
        const Vector3& posA, float radiusA,
        const Vector3& posB, float radiusB);

    // 球と球の反射処理
    void ReflectSphereVelocity(
        Vector3& position, Vector3& velocity,
        const Vector3& otherPos, float selfRadius, float otherRadius,
        float bounce = 1.1f // 反射倍率（デフォルト1.1倍）
    );

    Vector3 GetAnchorPosition();

    bool GetIsCut();

    void Initialize(Object3dManager* object3dManager);

    void Update(const char* keys, const char* preKeys, float deltaTime);

    void Draw(Object3dManager* object3dManager);

    void IsCollisionWall();

    void Reset();

    void VelocityReset();

    // ゲッターロボ
    const Vector3& GetPosition() const { return position_; }
    const float& GetRadius() const { return radius_; }
    const unsigned int& GetColor() const { return color_; }

    // セッター
    void SetVelocity(Vector3 velocity) { velocity_ = velocity; }
};
