#include "Quaternion.hpp"

#include "Vector3.hpp"
#include "../math.hpp"

/*operators*/
Quaternion Quaternion::operator+(const Quaternion& other) const noexcept
{
    return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}

Quaternion Quaternion::operator-(const Quaternion& other) const noexcept
{
    return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
}
Quaternion Quaternion::operator-() const noexcept
{
    return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::operator*(const ld other) const noexcept
{
    return Quaternion(w * other, x * other, y * other, z * other);
}
Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
{
    return Quaternion(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}
Vector3 Quaternion::operator*(const Vector3& other) const noexcept
{
    return Vector3(w * w * other.x + 2 * y * w * other.z - 2 * z * w * other.y + x * x * other.x + 2 * y * x * other.y + 2 * z * x * other.z - z * z * other.x - y * y * other.x,
    2 * x * y * other.x + y * y * other.y + 2 * z * y * other.z + 2 * w * z * other.x - z * z * other.y + w * w * other.y - 2 * x * w * other.z - x * x * other.y,
    2 * x * z * other.x + 2 * y * z * other.y + z * z * other.z - 2 * w * y * other.x - y * y * other.z + 2 * w * x * other.y - x * x * other.z + w * w * other.z
    );
}

Quaternion Quaternion::operator/(ld scalar) const noexcept
{
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}

Quaternion& Quaternion::operator+=(const Quaternion& q) noexcept
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}
Quaternion& Quaternion::operator-=(const Quaternion& other) noexcept
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

/*operations*/
ld Quaternion::norm() const noexcept
{
    return sqrtl(w * w + x * x + y * y + z * z);
}
Quaternion Quaternion::normalized() const noexcept
{
    ld n = norm();
    return Quaternion(w / n, x / n, y / n, z / n);
}

Quaternion Quaternion::conjugate() const noexcept
{
    return Quaternion(w, -x, -y, -z);
}
Quaternion Quaternion::inverse() const noexcept
{
    ld n = norm();
    return conjugate() / n;
}

/*rotate*/
Vector3 Quaternion::rotate(Vector3 v) const noexcept
{
    Quaternion vq(0, v);
    Quaternion rq = *this * vq * inverse();
    return Vector3(rq.x, rq.y, rq.z);
}
Vector3 Quaternion::rotate(ld length) const noexcept
{
    Quaternion vq(0, length, 0, 0);
    Quaternion rq = *this * vq * inverse();
    return Vector3(rq.x, rq.y, rq.z);
}

/*non-member functions*/
bool operator==(const Quaternion& l, const Quaternion& r) noexcept
{
    return l.w == r.w && l.x == r.x && l.y == r.y && l.z == r.z;
}
bool operator!=(const Quaternion& l, const Quaternion& r) noexcept
{
    return !(l == r);
}

Quaternion toQuaternion(const Vector3& rotation) noexcept
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

Quaternion getStepQuaternion(const Quaternion& orientation, const Quaternion& desiredOrientation, int n, ld maxRotationSpeed) noexcept {
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

ld getDifferenceRadian(const Quaternion& q, const Quaternion& p) noexcept {
    Quaternion step = (q - p);
    ld stepMagnitude = sqrt(step.x * step.x + step.y * step.y + step.z * step.z);
    return stepMagnitude;
}