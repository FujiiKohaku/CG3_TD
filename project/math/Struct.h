// 基本ベクトル.行列
#pragma once
struct Vector2 {
    float x, y;
};
struct Vector3 {
    float x, y, z;
};
struct Vector4 {
    float x, y, z, w;
};
struct Matrix3x3 {
    float m[3][3];
};
struct Matrix4x4 {
    float m[4][4];
};

// 当たり判定用
typedef struct Sphere {
    Vector3 center;// 中心点
    float radius;// 半径
}Sphere;

typedef struct AABB {
    Vector3 min;// 最小点
    Vector3 max;// 最大点
}AABB;