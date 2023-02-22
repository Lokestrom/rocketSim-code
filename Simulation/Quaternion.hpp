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
    Quaternion operator*(const Quaternion& other) const;
    Vector3 operator*(const Vector3& other);
    Quaternion operator/(double scalar) const;
    inline Vector3 rotate(Vector3 v) const;
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

inline Vector3 Quaternion::rotate(Vector3 v) const {
    Quaternion vq(0, v);
    Quaternion rq = *this * vq * inverse();
    return Vector3(rq.x, rq.y, rq.z);
}