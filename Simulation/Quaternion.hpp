#pragma once

#include "Vector3.hpp"

using ld = long double;

struct Quaternion {
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(ld w, ld x, ld y, ld z) : w(w), x(x), y(y), z(z) {}
    Quaternion(ld w, Vector3 vec) : w(w), x(vec.x), y(vec.y), z(vec.z) {}

    ld w, x, y, z;

    inline Quaternion conjugate() const;
    inline ld norm() const;
    Quaternion inverse() const;
    inline Quaternion normalized() const;

    inline Quaternion operator+(const Quaternion other) const;
    inline Quaternion& operator+=(const Quaternion& v);
    inline Quaternion operator*(const ld other) const;
    Quaternion operator*(const Quaternion& other) const;
    Vector3 operator*(const Vector3& other);
    Quaternion operator/(double scalar) const;

    inline Vector3 rotate(Vector3 v) const;
    inline Vector3 rotate(ld length) const;
};

inline Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

inline ld Quaternion::norm() const {
    return sqrtl(w * w + x * x + y * y + z * z);
}

inline Quaternion Quaternion::normalized() const {
    ld n = norm();
    return Quaternion(w / n, x / n, y / n, z / n);
}

inline Quaternion Quaternion::operator+(const Quaternion other) const {
    return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}

inline Quaternion& Quaternion::operator+=(const Quaternion& q) {
    this->w += q.w;
    this->x += q.x;
    this->y += q.y;
    this->z += q.z;
    return *this;
}

inline Quaternion Quaternion::operator*(const ld other) const {
    return Quaternion(w * other, x * other, y * other, z * other);
}

inline Vector3 Quaternion::rotate(Vector3 v) const {
    Quaternion vq(0, v);
    Quaternion rq = *this * vq * inverse();
    return Vector3(rq.x, rq.y, rq.z);
}

inline Vector3 Quaternion::rotate(ld length) const {
    Quaternion vq(0, length, 0,0);
    Quaternion rq = *this * vq * inverse();
    return Vector3(rq.x, rq.y, rq.z);
}

Quaternion toQuaternion(Vector3 rotation);