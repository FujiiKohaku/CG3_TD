#pragma once
#include "Struct.h"
#include "Object3d.h"
#include "Object3dManager.h"

class Block
{
	Vector3 centerPosition_ = { 0.0f,0.0f,0.0f };
	float width_ = 1.0f;
	float height_ = 1.0f;
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;
	float scaleZ_ = 1.0f;
	float bounce_ = 1.0f;

	AABB block_ = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };

	Object3d* object3d_ = nullptr;

public:

	void Initialize(Vector3 position, float width, float height, float bounce, Object3dManager* object3dManager, const std::string& modelName);

	void Draw();

	Vector3 Add(const Vector3& v1, const Vector3& v2);

	Vector3 Multiply(const float& i, const Vector3& v1);

	Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	float Dot(const Vector3& v1, const Vector3& v2);

	float Length(const Vector3& v);

	bool IsCollision(const AABB& aabb, const Sphere& spher);

	// デストラクタ
	~Block();

	// ボックスとの反射処理
	void ReflectSphereFromAABB(Vector3& position, Vector3& velocity, const AABB& aabb, float radius, float bounce);

	// ゲッターロボ
	const Vector3& GetPosition() const { return centerPosition_; }
	const float& GetBounce() const { return bounce_; }
	const AABB& GetBlockAABB()const { return block_; }

};

