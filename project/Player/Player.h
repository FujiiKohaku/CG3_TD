#pragma once
#include "Block.h"
#include "Bumper.h"
#include "Camera.h"
#include "Effect.h"
#include "Goal.h"
#include "Input.h"
#include "MatrixMath.h"
#include "Object3d.h"
#include "Object3dManager.h"
#include "Pendulum.h"
#include "ScoreBumper.h"
#include "SoundManager.h"
#include "StageSelectScene.h"
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
    float radius_ = 0.5f;
    unsigned int color_ = 0xFF0000FF;
    int point_ = 0;
    int clearPoint_ = 0;
    int isGoal_ = false;
    int releaseTimer_ = 600;
    int isReleaced_ = false;
    int isReleaceTimerStarted_ = false;

    Pendulum* pendulum_ = nullptr;
    Object3d* object3d_ = nullptr;
    Bumper* bumper_ = nullptr;
    Block* block_ = nullptr;
    Goal* goal_ = nullptr;

    // 壁の位置の初期化ほんとはここに入れるのは良くない本当によくない
    const float wallXMin = -20.0f;
    const float wallXMax = 20.0f;
    const float wallYMin = -20.0f;
    const float wallYMax = 20.0f;

    // 壁衝突処理（カプセル判定）
    Vector3 walls_[4][2] = {
        { { wallXMin, wallYMin, 0 }, { wallXMin, wallYMax, 0 } },
        { { wallXMax, wallYMin, 0 }, { wallXMax, wallYMax, 0 } },
        { { wallXMin, wallYMin, 0 }, { wallXMax, wallYMin, 0 } },
        { { wallXMin, wallYMax, 0 }, { wallXMax, wallYMax, 0 } }
    };

    Sphere playerSphere_;
    Sphere bumperSphere_;
    Sphere goalSphere_;
    AABB blockAABB_;

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

    Vector3 GetAnchorPosition();

    bool GetIsCut();

    void Initialize(int clearPoint, Object3dManager* object3dManager, const std::string& modelName);

    void Update(const char* keys, const char* preKeys, float deltaTime, Input* input);

    void Draw();

    void IsCollisionWall();

    void Reset();

    void VelocityReset();

    Pendulum* GetPendulum() const { return pendulum_; }

    // ゲッターロボ
    const Vector3& GetPosition() const { return position_; }
    const float& GetRadius() const { return radius_; }
    const unsigned int& GetColor() const { return color_; }
    const int GetPoint() const { return point_; }
    const int GetIsGoal() const { return isGoal_; }
    void AddScore(int score) { point_ += score; }
    // セッター
    void SetVelocity(Vector3 velocity) { velocity_ = velocity; }
    void SetBumper(Bumper* bumper) { bumper_ = bumper; }
    void SetBlockAABB(Block* block) { block_ = block; }
    void SetGoal(Goal* goal) { goal_ = goal; }
    void DrawWalls();
    void SetPosition(Vector3 position) { position_ = position; }
    float warpCooldown_ = 0.0f;
    void SetScoreBumper(ScoreBumper* scoreBumper) { scoreBumper_ = scoreBumper; }

private:
    Object3d* wallObjects_[4];
    Effect* effect_;
    ScoreBumper* scoreBumper_;
    SoundManager* sound_ = nullptr; // サウンドマネージャ参照
    SoundData hitSE_ {}; // 壁衝突音
    
};
