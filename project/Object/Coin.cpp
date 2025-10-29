#include "Coin.h"
#include <cmath>

void Coin::Initialize(Vector3 position, float scale, int score, Object3dManager* object3dManager, const std::string& modelName) {
	position_ = position;
	scale_ = scale;
	score_ = score;
	radius_ *= scale_ / 2.0f;

	object3d_ = new Object3d();
	object3d_->Initialize(object3dManager);
	object3d_->SetModel(modelName);
	object3d_->SetScale({ scale_, scale_, scale_ });
}

void Coin::Update() {
	if (!isActive_)return;

	position_ += velocity_;
	if (position_.x + radius_ > 20.0f || position_.x - radius_ < -20.0f) {
		velocity_.x *= -1.0f;
	}

	sphere_ = { position_, radius_ };

	// 回転などの見た目演出（少し回す）
	Vector3 rotate = object3d_->GetRotate();
	rotate.y += 0.1f;
	object3d_->SetRotate(rotate);
}

void Coin::Draw() {
	if (!isActive_)
		return;
	object3d_->SetTranslate(position_);
	object3d_->Update();
	object3d_->Draw();
}

bool Coin::IsCollision(const Sphere& playerSphere) {
	if (!isActive_)
		return false;

	Vector3 diff = {
		playerSphere.center.x - position_.x,
		playerSphere.center.y - position_.y,
		playerSphere.center.z - position_.z
	};
	float distance = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	if (distance <= playerSphere.radius + radius_) {
		isActive_ = false; // コイン消す
		return true;
	}
	return false;
}

Coin::~Coin() {
	if (object3d_) {
		delete object3d_;
		object3d_ = nullptr;
	}
}
