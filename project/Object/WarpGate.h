#pragma once
#include "Object3d.h"
#include "Object3dManager.h"
#include "Struct.h"

class Player; // 前方宣言（ヘッダー依存を防ぐ）

class WarpGate {
public:
    void Initialize(const Vector3& position, float scale, Object3dManager* object3dManager, const std::string& modelName);
    void Update();
    void Draw();

    // ペア設定（相方ワープゲートを指定）
    void SetPair(WarpGate* pair) { pair_ = pair; }

    // プレイヤーを渡してワープ判定する
    void CheckAndWarp(Player* player);

    bool IsCollision(const Sphere& playerSphere);
    const Vector3& GetPosition() const { return position_; }

private:
    Vector3 position_ {};
    float scale_ = 1.0f;
    float radius_ = 1.0f;
    Sphere sphere_ {};
    Object3d* object3d_ = nullptr;
    WarpGate* pair_ = nullptr; // ワープ先（ペア）
};
