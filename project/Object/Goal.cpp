#include "Goal.h"

void Goal::Initialize(Vector3 position, float scale, Object3dManager* object3dManager, const std::string& modelName)
{
	position_ = position;
	scale_ = scale;
	radius_ *= scale_ / 2.0f;
	object3d_ = new Object3d;
	object3d_->Initialize(object3dManager);
	object3d_->SetModel(modelName);
}

void Goal::Update()
{
	sphre_ = { position_,radius_ };
}

void Goal::Draw()
{
	// 現在の位置に合わせる
	object3d_->SetTranslate(position_);
	object3d_->Update();
	object3d_->Draw();
	object3d_->SetScale({ scale_, scale_, scale_ });
}

float Goal::Length(const Vector3& v)
{
	float length;

	length = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));

	return length;
}

bool Goal::IsCollision(const Sphere& s1, const Sphere& s2)
{
	if (isActive_) {
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
}

Goal::~Goal()
{
	if (object3d_) {
		delete object3d_;
		object3d_ = nullptr;
	}
}
