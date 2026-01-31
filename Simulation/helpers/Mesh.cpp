#include "Mesh.hpp"
//#include "simulationObjects.hpp"
#include "../rocket/SimulationObject.hpp"

#include <algorithm>
#include <unordered_set>

BoundingSphre::BoundingSphre(const Vector<Vector3>& vertices)
{
    Vector3 max = {LDBL_MIN, LDBL_MIN, LDBL_MIN },
            min = {LDBL_MAX, LDBL_MAX, LDBL_MAX };

    for (auto& i : vertices) {
        max = { std::max(max.x, i.x),
            std::max(max.y, i.y),
            std::max(max.z, i.z)
        };
        min = { std::min(min.x, i.x),
            std::min(min.y, i.y),
            std::min(min.z, i.z)
        };
    }
    Vector3 dif = max - min;

    _radius = std::max(dif.x, std::max(dif.y, dif.z));
}

void BoundingSphre::isInsideOtherBoundingSphre(const Vector3& pos, Vector<ID::UUID>& ids)
{
    for (auto& i : objectLists::simulationObjects) {
        if ((pos - i->transform->translation).lengthSquared() < (i->boundingSphre._radius + _radius) * (i->boundingSphre._radius + _radius)) {
            ids.pushBack(i->id.getUUID());
        }
    }
}

Vector<Vector2> getFacing2DBoundry(const SimulationModel& model, const Vector3& dir) {
    Vector<Vector2> transformedVertices;

    const Vector3 u{ dir.cross({0,0,1}) };
    const Vector3 v{ dir.cross(u) };

    double a = sqrt(1.0 + u.x + v.y + dir.z) / 2.0;
    double x = (v.z - dir.y) / (4.0 * a);
    double y = (dir.x - u.z) / (4.0 * a);
    double z = (u.y - v.x) / (4.0 * a);

    Quaternion rotation = Quaternion{ a,x,y,z };
    
    int maxIndex;
    ld max = LDBL_MIN;
    Vector3 rotatedPoint;
    for (auto& i : model.vertices) {
        rotatedPoint = rotation.rotate(i);
        transformedVertices.pushBack({ rotatedPoint.x, rotatedPoint.y });
        if (transformedVertices[transformedVertices.size() - 1].x > max) {
            max = transformedVertices[transformedVertices.size() - 1].x;
            maxIndex = transformedVertices.size() - 1;
        }
    }
    
    std::unordered_map<Vector2, std::unordered_set<Vector2>> connections;
    for (auto i = 0; i < transformedVertices.size(); i++) {
        connections[transformedVertices[i]].insert(transformedVertices[(i + 1) % transformedVertices.size()]);
        connections[transformedVertices[i]].insert(transformedVertices[(i + 2) % transformedVertices.size()]);
    }
    return Vector<Vector2>();
}

ld getFacingSurfaceArea(const SimulationModel& model, const Vector3& dir)
{
    Vector<Vector2> vertices = getFacing2DBoundry(model, dir);
    ld area = 0;
    int j;
    for (auto i = 0; i < vertices.size(); i++) {
        j = i + 1 % vertices.size();
        area += vertices[i].x * vertices[j].y;
        area -= vertices[i].y * vertices[j].x;
    }
    return area;
}
