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

// ================================
// Vector3 演算子オーバーロード
// ================================
inline Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline Vector3 operator*(const Vector3& v, float s)
{
    return { v.x * s, v.y * s, v.z * s };
}

inline Vector3 operator*(float s, const Vector3& v)
{
    return { v.x * s, v.y * s, v.z * s };
}

inline Vector3 operator/(const Vector3& v, float s)
{
    return { v.x / s, v.y / s, v.z / s };
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

inline Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

inline Vector3& operator*=(Vector3& v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

inline Vector3& operator/=(Vector3& v, float s)
{
    v.x /= s;
    v.y /= s;
    v.z /= s;
    return v;
}
