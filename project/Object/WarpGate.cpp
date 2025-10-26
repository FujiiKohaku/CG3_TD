#include "WarpGate.h"
#include "Player.h"
#include <cmath>

void WarpGate::Initialize(const Vector3& position, float scale, Object3dManager* object3dManager, const std::string& modelName)
{
    position_ = position;
    scale_ = scale;
    radius_ = scale_ * 0.5f;

    object3d_ = new Object3d();
    object3d_->Initialize(object3dManager);
    object3d_->SetModel(modelName);
    object3d_->SetScale({ scale_, scale_, scale_ });
    object3d_->SetTranslate(position_);
}

void WarpGate::Update()
{
    Vector3 rot = object3d_->GetRotate();
    rot.y += 0.05f;
    object3d_->SetRotate(rot);
    sphere_ = { position_, radius_ };
}

void WarpGate::Draw()
{
    object3d_->Update();
    object3d_->Draw();
}

bool WarpGate::IsCollision(const Sphere& playerSphere)
{
    Vector3 diff = {
        playerSphere.center.x - position_.x,
        playerSphere.center.y - position_.y,
        playerSphere.center.z - position_.z
    };
    float dist = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    return dist <= (playerSphere.radius + radius_);
}

void WarpGate::CheckAndWarp(Player* player)
{
    if (!pair_)
        return; // ペア未設定なら無視
    if (player->warpCooldown_ > 0.0f)
        return; // クールダウン中なら無視

    Sphere playerSphere = { player->GetPosition(), player->GetRadius() };
    if (IsCollision(playerSphere)) {
        // ワープ実行
        player->SetPosition(pair_->GetPosition());

        // クールダウン設定（約1秒）
        player->warpCooldown_ = 1.0f;
    }
}
