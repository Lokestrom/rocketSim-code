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

Vector3 Quaternion::operator*(const Vector3& other){
    return Vector3(w * w * other.x + 2 * y * w * other.z - 2 * z * w * other.y + x * x * other.x + 2 * y * x * other.y + 2 * z * x * other.z - z * z * other.x - y * y * other.x,
    2 * x * y * other.x + y * y * other.y + 2 * z * y * other.z + 2 * w * z * other.x - z * z * other.y + w * w * other.y - 2 * x * w * other.z - x * x * other.y,
    2 * x * z * other.x + 2 * y * z * other.y + z * z * other.z - 2 * w * y * other.x - y * y * other.z + 2 * w * x * other.y - x * x * other.z + w * w * other.z
    );
}


Quaternion Quaternion::operator/(double scalar) const {
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}
