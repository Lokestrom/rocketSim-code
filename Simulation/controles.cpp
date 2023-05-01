#include "controles.hpp"

ld fixAngle360(ld angle){
    if (angle >= 360)
        return findRest(angle, 360);

    if (angle < 0)
    {
        while (angle < 0)
        {
            angle += 360;
        }
        return angle;
    }
    return angle;
}

ld fixAngle180(ld angle){
    if (angle >= 180)
        return findRest(angle, 180);

    if (angle < 0)
    {
        while (angle < 0)
        {
            angle += 180;
        }
        return angle;
    }
    return angle;
}

Vector3 generateGravity(ld m, ld M, Vector3 pos, Vector3 otherPos) {
    return gravityFormula(m, M, distanse(pos, otherPos)) * (otherPos - pos).normal();
}
