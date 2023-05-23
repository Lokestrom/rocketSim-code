#pragma once

#include "Vector3.hpp"

struct Quaternion {
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(ld w, ld x, ld y, ld z) : w(w), x(x), y(y), z(z) {}
    Quaternion(ld w, Vector3 vec) : w(w), x(vec.x), y(vec.y), z(vec.z) {}

    ld w, x, y, z;

    inline Quaternion conjugate() const;
    inline ld norm() const;
    Quaternion inverse() const;
    inline Quaternion normalized() const;

    inline Quaternion operator+(const Quaternion& other) const;
    inline Quaternion& operator+=(const Quaternion& v);
    inline Quaternion operator-(const Quaternion& other) const;
    inline Quaternion operator-() const;
    inline Quaternion& operator-=(const Quaternion& other);
    inline Quaternion operator*(const ld other) const;
    Quaternion operator*(const Quaternion& other) const;
    Vector3 operator*(const Vector3& other) const;
    Quaternion operator/(ld scalar) const;

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

inline Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}

inline Quaternion& Quaternion::operator+=(const Quaternion& q) {
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
}

Quaternion Quaternion::operator-() const {
    return Quaternion(-w, -x, -y, -z);
}

Quaternion& Quaternion::operator-=(const Quaternion& other) {
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

inline Quaternion Quaternion::operator*(const ld other) const {
    return Quaternion(w * other, x * other, y * other, z * other);
}

inline bool operator==(const Quaternion l, const Quaternion r) {
    return l.w == r.w && l.x == r.x && l.y == r.y && l.z == r.z;
}

inline bool operator!=(const Quaternion l, const Quaternion r) {
    return !(l == r);
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

Quaternion getStepQuaternion(Quaternion orientation, Quaternion desiredOrientation, int n, ld maxRotationSpeed) {
    Quaternion step = (desiredOrientation - orientation) / n;
    ld stepMagnitude = sqrt(step.x * step.x + step.y * step.y + step.z * step.z);
    if (stepMagnitude > maxRotationSpeed) {
        ld scale = maxRotationSpeed / stepMagnitude;
        step.x *= scale;
        step.y *= scale;
        step.z *= scale;
    }
    return step;
}

ld getDifferenceRadian(Quaternion q, Quaternion p) {
    Quaternion step = (q - p);
    ld stepMagnitude = sqrt(step.x * step.x + step.y * step.y + step.z * step.z);
    return stepMagnitude;
}