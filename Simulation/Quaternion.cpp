#include "Quaternion.hpp"
#include "Vector3.hpp"

Quaternion Quaternion::inverse() const {
    ld n = norm();
    return conjugate() / n;
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}

Vector3 Quaternion::operator*(const Vector3& other) const {
    return Vector3(w * w * other.x + 2 * y * w * other.z - 2 * z * w * other.y + x * x * other.x + 2 * y * x * other.y + 2 * z * x * other.z - z * z * other.x - y * y * other.x,
    2 * x * y * other.x + y * y * other.y + 2 * z * y * other.z + 2 * w * z * other.x - z * z * other.y + w * w * other.y - 2 * x * w * other.z - x * x * other.y,
    2 * x * z * other.x + 2 * y * z * other.y + z * z * other.z - 2 * w * y * other.x - y * y * other.z + 2 * w * x * other.y - x * x * other.z + w * w * other.z
    );
}


Quaternion Quaternion::operator/(ld scalar) const {
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}

Quaternion toQuaternion(Vector3 rotation)
{
    ld cr = cosl(rotation.x * 0.5);
    ld sr = sinl(rotation.x * 0.5);
    ld cp = cosl(rotation.y * 0.5);
    ld sp = sinl(rotation.y * 0.5);
    ld cy = cosl(rotation.z * 0.5);
    ld sy = sinl(rotation.z * 0.5);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}