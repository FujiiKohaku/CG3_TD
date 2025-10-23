#pragma once
#include "Struct.h"
#include "Object3d.h"
#include "Object3dManager.h"

class Goal
{
    Vector3 position_ = { 0.0f,0.0f,0.0f };
    float radius_ = 1.0f;
    float scale_ = 1.0f;
    Object3d* object3d_ = nullptr;

    int isActive_ = false;

    Sphere sphre_;

public:

    void Initialize(Vector3 position, float scale, Object3dManager* object3dManager, const std::string& modelName);

    void Update();

    void Draw();

    float Length(const Vector3& v);

    // ���Ƌ��̏Փ˔���
    bool IsCollision(const Sphere& s1, const Sphere& s2);

    ~Goal();

    // �Q�b�^�[���{
    const Vector3& GetPosition() const { return position_; }
    const float& GetRadius() const { return radius_; }
    const int& GetIsActive() const { return isActive_; }

    // �Z�b�^�[
    void SetPosition(Vector3 position) { position_ = position; }
    void SetIsActive(int isActive) { isActive_ = isActive; }
};

