#include "physics.hpp"
#include "math.hpp"

ld gravityFormulaNewton(ld m, ld M, ld r)
{
    return (G * m * M) / (r * r);
}
Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos) {
    Vector3 relativePos = otherPos - pos;
    if (relativePos == Vector3::null())
        return Vector3::null();
    return ((G * m * M) / relativePos.lengthSquared()) * relativePos.normal();
}
