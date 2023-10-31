#include "physics.hpp"
#include "math.hpp"

ld gravityFormulaNewton(ld m, ld M, ld r)
{
    return (G * m * M) / (r * r);
}
Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos) {
    if (pos == otherPos)
        return Vector3::null();
    return gravityFormulaNewton(m, M, distanse(pos, otherPos)) * (otherPos - pos).normal();
}
