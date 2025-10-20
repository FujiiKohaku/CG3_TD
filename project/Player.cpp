#include "Player.h"
Vector3 Player::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 Player::Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

Vector3 Player::Multiply(const float& i, const Vector3& v1)
{
	Vector3 result;

	result.x = v1.x * i;
	result.y = v1.y * i;
	result.z = v1.z * i;

	return result;
}

Matrix4x4 Player::Add(Matrix4x4 matrix1, Matrix4x4 matrix2)
{
	Matrix4x4 result;

	result.m[0][0] = matrix1.m[0][0] + matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] + matrix2.m[0][1];
	result.m[0][2] = matrix1.m[0][2] + matrix2.m[0][2];
	result.m[0][3] = matrix1.m[0][3] + matrix2.m[0][3];

	result.m[1][0] = matrix1.m[1][0] + matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] + matrix2.m[1][1];
	result.m[1][2] = matrix1.m[1][2] + matrix2.m[1][2];
	result.m[1][3] = matrix1.m[1][3] + matrix2.m[1][3];

	result.m[2][0] = matrix1.m[2][0] + matrix2.m[2][0];
	result.m[2][1] = matrix1.m[2][1] + matrix2.m[2][1];
	result.m[2][2] = matrix1.m[2][2] + matrix2.m[2][2];
	result.m[2][3] = matrix1.m[2][3] + matrix2.m[2][3];

	result.m[3][0] = matrix1.m[3][0] + matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][1] + matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][2] + matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][3] + matrix2.m[3][3];

	return result;
}

Matrix4x4 Player::Subtract(Matrix4x4 matrix1, Matrix4x4 matrix2)
{
	Matrix4x4 result;

	result.m[0][0] = matrix1.m[0][0] - matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] - matrix2.m[0][1];
	result.m[0][2] = matrix1.m[0][2] - matrix2.m[0][2];
	result.m[0][3] = matrix1.m[0][3] - matrix2.m[0][3];

	result.m[1][0] = matrix1.m[1][0] - matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] - matrix2.m[1][1];
	result.m[1][2] = matrix1.m[1][2] - matrix2.m[1][2];
	result.m[1][3] = matrix1.m[1][3] - matrix2.m[1][3];

	result.m[2][0] = matrix1.m[2][0] - matrix2.m[2][0];
	result.m[2][1] = matrix1.m[2][1] - matrix2.m[2][1];
	result.m[2][2] = matrix1.m[2][2] - matrix2.m[2][2];
	result.m[2][3] = matrix1.m[2][3] - matrix2.m[2][3];

	result.m[3][0] = matrix1.m[3][0] - matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][1] - matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][2] - matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][3] - matrix2.m[3][3];

	return result;
}

float Player::LengthSq(const Vector3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Player::Length(const Vector3& v)
{
	float length;

	length = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));

	return length;
}

bool Player::isOutOfBounds()
{
	if (position_.x < wallXMin || position_.x > wallXMax || position_.y < wallYMin || position_.y > wallYMax) {
		return true;
	}

	return false;
}

bool Player::SphereIntersectsSphere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB)
{
	Vector3 diff = { posA.x - posB.x, posA.y - posB.y, posA.z - posB.z };
	float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	float r = radiusA + radiusB;
	return distSq < (r * r);
}

void Player::ReflectSphereVelocity(Vector3& position, Vector3& velocity, const Vector3& otherPos, float selfRadius, float otherRadius, float bounce)
{
	Vector3 diff = { position.x - otherPos.x, position.y - otherPos.y, position.z - otherPos.z };
	float dist = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	float sumRadius = selfRadius + otherRadius;

	if (dist < sumRadius && dist > 1e-6f) {
		// ���K���@��
		Vector3 n = { diff.x / dist, diff.y / dist, diff.z / dist };

		// �߂荞�ݕ␳
		float penetration = sumRadius - dist;
		position.x += n.x * penetration;
		position.y += n.y * penetration;
		position.z += n.z * penetration;

		// ���ˏ���
		float dotN = velocity.x * n.x + velocity.y * n.y + velocity.z * n.z;
		velocity.x -= 2.0f * dotN * n.x;
		velocity.y -= 2.0f * dotN * n.y;
		velocity.z -= 2.0f * dotN * n.z;

		// ���˔{��
		velocity.x *= bounce;
		velocity.y *= bounce;
		velocity.z *= bounce;
	}
}

Vector3 Player::GetAnchorPosition()
{
	anchorPosition_ = pendulum_->GetAnchor();

	return anchorPosition_;
}

bool Player::GetIsCut()
{
	bool isCut_;

	isCut_ = pendulum_->GetIsCut();

	return isCut_;
}

void Player::Initialize()
{
	pendulum_ = new Pendulum;
}

void Player::Update(const char* keys, const char* preKeys, float deltaTime)
{
	if (!pendulum_->GetIsCut()) {
		// �U��q���[�h
		pendulum_->Update(keys, preKeys, deltaTime);

		// ���ʒu�X�V
		position_.x = pendulum_->GetAnchor().x + sinf(pendulum_->GetAngle()) * pendulum_->GetLength();
		position_.y = pendulum_->GetAnchor().y - cosf(pendulum_->GetAngle()) * pendulum_->GetLength();
		position_.z = pendulum_->GetAnchor().z;

		if (keys[DIK_R] && !preKeys[DIK_R]) {
			// �ؒf����
			pendulum_->SetPrevAnchorPos(pendulum_->GetLastAnchor());
			pendulum_->SetLastAnchorPos(pendulum_->GetAnchor());

			int count = pendulum_->GetStrokeCount();
			pendulum_->SetStrokeCount(++count);
			float speedX = pendulum_->GetAngle() * pendulum_->GetLength() * cosf(pendulum_->GetAngle());
			float speedY = pendulum_->GetAngle() * pendulum_->GetLength() * sinf(pendulum_->GetAngle());
			velocity_ = { speedX, speedY, 0.0f };

			pendulum_->SetIsCut(true);
		}
	} else {
		// �������[�h
		Vector3 prevPos = position_;
		position_ = Add(position_, Multiply(deltaTime, velocity_));

		// �ǏՓˏ����i�J�v�Z������j
		Vector3 walls[4][2] = {
			{{wallXMin, wallYMin,0},{wallXMin,wallYMax,0}},
			{{wallXMax, wallYMin,0},{wallXMax,wallYMax,0}},
			{{wallXMin, wallYMin,0},{wallXMax,wallYMin,0}},
			{{wallXMin, wallYMax,0},{wallXMax,wallYMax,0}}
		};
		for (int i = 0; i < 4; i++) {
			if (CapsuleIntersectsSegment3D(prevPos, position_, radius_, walls[i][0], walls[i][1])) {
				Vector3 normal;
				switch (i) {
				case 0: normal = { 1,0,0 }; break;
				case 1: normal = { -1,0,0 }; break;
				case 2: normal = { 0,1,0 }; break;
				case 3: normal = { 0,-1,0 }; break;
				}
				position_ = prevPos;
				float dotN = Dot(velocity_, normal);
				velocity_ = Subtract(velocity_, Multiply(2.0f * dotN, normal));
				break;
			}
		}

		// �o���p�[�Ƃ̔���
		if (SphereIntersectsSphere(position_, radius_, bumperPos_, bumperRadius_)) {
			ReflectSphereVelocity(position_, velocity_, bumperPos_, radius_, bumperRadius_, bounce_);
		}

		// ����
		velocity_ = Multiply(decelerationRate_, velocity_);

		if (Length(velocity_) < 0.05f) {
			if (isOutOfBounds()) {

				int count = pendulum_->GetStrokeCount();
				pendulum_->SetStrokeCount(++count);
				pendulum_->SetAnchorPos(pendulum_->GetPrevAnchor());
				position_ = { pendulum_->GetAnchor().x, pendulum_->GetAnchor().y - pendulum_->GetLength(), 0.0f };
			} else {
				pendulum_->SetAnchorPos({ position_.x, position_.y + pendulum_->GetLength(), position_.z });
			}
			pendulum_->Reset();
			pendulum_->SetIsCut(false);
		}
	}
}

void Player::IsCollisionWall()
{
	for (int i = 0; i < 4; i++) {
		if (CapsuleIntersectsSegment3D(prevPosition_, position_, radius_, walls_[i][0], walls_[i][1])) {
			Vector3 normal;
			switch (i) {
			case 0: normal = { 1,0,0 }; break;
			case 1: normal = { -1,0,0 }; break;
			case 2: normal = { 0,1,0 }; break;
			case 3: normal = { 0,-1,0 }; break;
			}
			position_ = prevPosition_;
			float dotN = Dot(velocity_, normal);
			velocity_ = Subtract(velocity_, Multiply(2.0f * dotN, normal));
			break;
		}
	}
}

void Player::Reset()
{
	position_ = { 0.0f,0.0f,0.0f };
	VelocityReset();
}

void Player::VelocityReset()
{
	velocity_ = { 0.0f, 0.0f, 0.0f };
	pendulum_->Reset();
}

bool Player::CapsuleIntersectsSegment3D(const Vector3& capsuleStart, const Vector3& capsuleEnd, float radius, const Vector3& segStart, const Vector3& segEnd)
{
	// �J�v�Z��������
	Vector3 u = Subtract(capsuleEnd, capsuleStart);
	Vector3 v = Subtract(segEnd, segStart);
	Vector3 w = Subtract(capsuleStart, segStart);

	float a = Dot(u, u);  // �J�v�Z�����̒���?
	float b = Dot(u, v);
	float c = Dot(v, v);
	float d = Dot(u, w);
	float e = Dot(v, w);

	float denom = a * c - b * b;
	float sc, sN, sD = denom;
	float tc, tN, tD = denom;

	// �܂������Ԃ̍ŋߐړ_�p�����[�^ s, t �����߂�i����-���������̕W�����j
	const float EPS = 1e-6f;
	if (denom < EPS) {
		sN = 0.0f;  // ���s��
		sD = 1.0f;
		tN = e;
		tD = c;
	} else {
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0f) {
			sN = 0.0f;
			tN = e;
			tD = c;
		} else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	tc = (fabsf(tN) < EPS ? 0.0f : tN / tD);
	tc = std::clamp(tc, 0.0f, 1.0f);
	sc = (fabsf(sN) < EPS ? 0.0f : sN / sD);
	sc = std::clamp(sc, 0.0f, 1.0f);

	// �����Ԃ̍ŋߐړ_�����߂�
	Vector3 p1 = Add(capsuleStart, Multiply(sc, u));
	Vector3 p2 = Add(segStart, Multiply(tc, v));

	// ���������a�ȉ��Ȃ����
	float distSq = LengthSq(Subtract(p1, p2));
	return distSq <= (radius * radius);
}

float Player::Dot(const Vector3& v1, const Vector3& v2)
{
	float resoult;

	resoult = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return resoult;
}