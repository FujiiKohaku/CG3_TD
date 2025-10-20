#include "Pendulum.h"

void Pendulum::Initialize()
{
}

void Pendulum::Update(const char* keys, const char* preKeys, float deltaTime)
{
    if (isCut_) return; // �ؒf��͏������Ȃ�

    // SPACE�ŉ���
    if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
        angularVelocity_ += kickStrength_;
    }

    if (angularVelocity_ > angularVelocityMax_) angularVelocity_ = angularVelocityMax_;

    // �p�����x�i�������݁j
    angularAcceleration_ = (-9.8f / length_) * sinf(angle_) - damping_ * angularVelocity_;
    angularVelocity_ += angularAcceleration_ * deltaTime;
    angle_ += angularVelocity_ * deltaTime;
}

void Pendulum::Reset()
{
    // �����p�x�E���x�����Z�b�g
    angle_ = 0.0f;
    angularVelocity_ = 0.0f;
    angularAcceleration_ = 0.0f;
    isCut_ = false;
}
