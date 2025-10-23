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

void Player::Initialize(Object3dManager* object3dManager, const std::string& modelName)
{
    pendulum_ = new Pendulum;
    object3d_ = new Object3d;
    object3d_->Initialize(object3dManager);
    object3d_->SetModel(modelName);

    // 壁モデルを作成
    for (int i = 0; i < 4; i++) {
        wallObjects_[i] = new Object3d;
        wallObjects_[i]->Initialize(object3dManager);
        wallObjects_[i]->SetModel("cube.obj");
    }

    // 壁の座標設定
    walls_[0][0] = { wallXMin, wallYMin, 0 };
    walls_[0][1] = { wallXMin, wallYMax, 0 };

    walls_[1][0] = { wallXMax, wallYMin, 0 };
    walls_[1][1] = { wallXMax, wallYMax, 0 };

    walls_[2][0] = { wallXMin, wallYMin, 0 };
    walls_[2][1] = { wallXMax, wallYMin, 0 };

    walls_[3][0] = { wallXMin, wallYMax, 0 };
    walls_[3][1] = { wallXMax, wallYMax, 0 };
}

void Player::Update(const char* keys, const char* preKeys, float deltaTime, Input* input)
{
    if (!pendulum_->GetIsCut()) {
        // 振り子モード
        pendulum_->Update(keys, preKeys, deltaTime, input);

        // 球位置更新
        position_.x = pendulum_->GetAnchor().x + sinf(pendulum_->GetAngle()) * pendulum_->GetLength();
        position_.y = pendulum_->GetAnchor().y - cosf(pendulum_->GetAngle()) * pendulum_->GetLength();
        position_.z = pendulum_->GetAnchor().z;

        if (input->IsKeyTriggered(DIK_R)) {
            // 切断処理
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
        // 慣性モード
        Vector3 prevPos = position_;
        position_ = Add(position_, Multiply(deltaTime, velocity_));

        // 壁衝突処理（カプセル判定）
        Vector3 walls[4][2] = {
            { { wallXMin, wallYMin, 0 }, { wallXMin, wallYMax, 0 } },
            { { wallXMax, wallYMin, 0 }, { wallXMax, wallYMax, 0 } },
            { { wallXMin, wallYMin, 0 }, { wallXMax, wallYMin, 0 } },
            { { wallXMin, wallYMax, 0 }, { wallXMax, wallYMax, 0 } }
        };
        for (int i = 0; i < 4; i++) {
            if (CapsuleIntersectsSegment3D(prevPos, position_, radius_, walls[i][0], walls[i][1])) {
                Vector3 normal;
                switch (i) {
                case 0:
                    normal = { 1, 0, 0 };
                    break;
                case 1:
                    normal = { -1, 0, 0 };
                    break;
                case 2:
                    normal = { 0, 1, 0 };
                    break;
                case 3:
                    normal = { 0, -1, 0 };
                    break;
                }
                position_ = prevPos;
                float dotN = Dot(velocity_, normal);
                velocity_ = Subtract(velocity_, Multiply(2.0f * dotN, normal));
                break;
            }
        }

        // バンパーとの当たり判定と反射
        playerSphere_ = { position_, radius_ };
        bumperSphere_ = { bumper_->GetPosition(), bumper_->GetRadius() };
        if (bumper_->IsCollision(playerSphere_, bumperSphere_)) {
            point_ += 100;
            bumper_->ReflectSphereVelocity(playerSphere_, velocity_, bumperSphere_);
            position_ = playerSphere_.center;
        }
       
        // ブロックとの当たり判定と反射
        blockAABB_ = { block_->GetBlockAABB() };
        if (block_->IsCollision(blockAABB_, playerSphere_)) {
            point_ += 100;
            block_->ReflectSphereFromAABB(position_, velocity_, blockAABB_, radius_, block_->GetBounce());
        }

        // ゴールとの当たり判定
        if (point_ >= clearPoint_) {
            goal_->SetIsActive(true);
            goalSphere_ = { goal_->GetPosition(),goal_->GetRadius() };
            if (goal_->IsCollision(playerSphere_, goalSphere_)) {
                isGoal_ = true;
            }
        }

        // 減速
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

void Player::Draw()
{
    // 現在の位置に合わせる
    object3d_->SetTranslate(position_);
    object3d_->Update();
    object3d_->Draw();

    DrawWalls();
}

void Player::IsCollisionWall()
{
    for (int i = 0; i < 4; i++) {
        if (CapsuleIntersectsSegment3D(prevPosition_, position_, radius_, walls_[i][0], walls_[i][1])) {
            Vector3 normal;
            switch (i) {
            case 0:
                normal = { 1, 0, 0 };
                break;
            case 1:
                normal = { -1, 0, 0 };
                break;
            case 2:
                normal = { 0, 1, 0 };
                break;
            case 3:
                normal = { 0, -1, 0 };
                break;
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
    position_ = { 0.0f, 0.0f, 0.0f };
    VelocityReset();
}

void Player::VelocityReset()
{
    velocity_ = { 0.0f, 0.0f, 0.0f };
    pendulum_->Reset();
}



bool Player::CapsuleIntersectsSegment3D(const Vector3& capsuleStart, const Vector3& capsuleEnd, float radius, const Vector3& segStart, const Vector3& segEnd)
{
    // カプセル軸方向
    Vector3 u = Subtract(capsuleEnd, capsuleStart);
    Vector3 v = Subtract(segEnd, segStart);
    Vector3 w = Subtract(capsuleStart, segStart);

    float a = Dot(u, u); // カプセル軸の長さ?
    float b = Dot(u, v);
    float c = Dot(v, v);
    float d = Dot(u, w);
    float e = Dot(v, w);

    float denom = a * c - b * b;
    float sc, sN, sD = denom;
    float tc, tN, tD = denom;

    // まず線分間の最近接点パラメータ s, t を求める（線分-線分距離の標準式）
    const float EPS = 1e-6f;
    if (denom < EPS) {
        sN = 0.0f; // 平行時
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

    // 線分間の最近接点を求める
    Vector3 p1 = Add(capsuleStart, Multiply(sc, u));
    Vector3 p2 = Add(segStart, Multiply(tc, v));

    // 距離が半径以下なら交差
    float distSq = LengthSq(Subtract(p1, p2));
    return distSq <= (radius * radius);
}

Player::~Player()
{
    if (pendulum_) {
        delete pendulum_;
        pendulum_ = nullptr;
    }
    if (object3d_) {
        delete object3d_;
        object3d_ = nullptr;
    }
    for (int i = 0; i < 4; i++) {
        if (wallObjects_[i]) {
            delete wallObjects_[i];
            wallObjects_[i] = nullptr;
        }
    }
}

float Player::Dot(const Vector3& v1, const Vector3& v2)
{
    float resoult;

    resoult = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    return resoult;
}

void Player::DrawWalls()
{
    for (int i = 0; i < 4; i++) {
        Vector3 start = walls_[i][0];
        Vector3 end = walls_[i][1];
        Vector3 center = {
            (start.x + end.x) / 2.0f,
            (start.y + end.y) / 2.0f,
            (start.z + end.z) / 2.0f
        };

        // 壁の長さ（スケールに反映）
        float length = Length(Subtract(end, start));
        Vector3 scale;
        if (i < 2) {
            scale = { 0.1f, length, 0.1f }; // 縦の壁
        } else {
            scale = { length, 0.1f, 0.1f }; // 横の壁
        }

        wallObjects_[i]->SetTranslate(center);
        wallObjects_[i]->SetScale(scale);
        wallObjects_[i]->Update();
        wallObjects_[i]->Draw();
    }
}

