#include "Mesh.hpp"
#include <algorithm>

ld Sphere::surfaceArea() const noexcept
{
    return 4 * PI * radius * radius;
}
ld Sphere::volum() const noexcept
{
    return (4/3) * PI * radius * radius * radius;
}
Vector<Vector3> Sphere::pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept
{
    n *= surfaceArea();
    Vector<Vector3> ans(n);
    ld phi;
    ld theta;
    for (int i = 0; i < n; i++) {
        phi = acos(1 - 2 * (i+0.5) / n);
        theta = PI * (1 + sqrt(5)) * (i+0.5);

        ans.pushBack({ cosl(theta) * sinl(phi), sinl(theta) * sinl(phi), cosl(phi) });
    }
    return ans;
}
bool Sphere::pointInside(const Vector3& point) const noexcept{
    Vector3 relative = point - pos;
    return relative.length() < radius;
}


ld Box::surfaceArea() const noexcept
{
    return (dimensions.x * dimensions.y * 2) + (dimensions.x * dimensions.z * 2) + (dimensions.y * dimensions.z * 2);
}
ld Box::volum() const noexcept
{
    return dimensions.x * dimensions.y * dimensions.z;
}
Vector<Vector3> Box::pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept
{
    n *= surfaceArea();
    Vector<Vector3> ans(n);

    sizeT x = sqrt((dimensions.x * n) / dimensions.length()),
        y = sqrt((dimensions.y * n) / dimensions.length()),
        z = sqrt((dimensions.z * n) / dimensions.length());
    

    Vector3 halfDimentions = dimensions / 2;

    /*x top*/
    for (auto j = 0; j < y; j++) {
        for (auto k = 0; k < z; k++)
            ans.pushBack(orientation.rotate({ halfDimentions.x, j / (n * dimensions.y) - halfDimentions.y, k / (n * dimensions.z) - halfDimentions.z }) + pos);
    }
    /*x bottom*/
    for (auto j = 0; j < y; j++) {
        for (auto k = 0; k < z; k++)
            ans.pushBack(orientation.rotate({ -halfDimentions.x, j / (n * dimensions.y) - halfDimentions.y, k / (n * dimensions.z) - halfDimentions.z }) + pos);
    }
    /*y top*/
    for (auto i = 0; i < x; i++) {
        for (auto k = 0; k < z; k++)
            ans.pushBack(orientation.rotate({ i / (n * dimensions.x) - halfDimentions.x, halfDimentions.y, k / (n * dimensions.z) - halfDimentions.z }) + pos);
    }
    /*y bottom*/
    for (auto i = 0; i < x; i++) {
        for (auto k = 0; k < z; k++)
            ans.pushBack(orientation.rotate({ i / (n * dimensions.x) - halfDimentions.x, -halfDimentions.y, k / (n * dimensions.z) - halfDimentions.z }) + pos);
    }
    /*z top*/
    for (auto i = 0; i < x; i++) {
        for (auto j = 0; j < y; j++)
            ans.pushBack(orientation.rotate({ i / (n * dimensions.x) - halfDimentions.x, j / (n * dimensions.y) - halfDimentions.y, halfDimentions.z }) + pos);
    }
    /*z bottom*/
    for (auto i = 0; i < x; i++) {
        for (auto j = 0; j < y; j++)
            ans.pushBack(orientation.rotate({ i / (n * dimensions.x) - halfDimentions.x, j / (n * dimensions.y) - halfDimentions.y, -halfDimentions.z }) + pos);
    }

    return ans;
}
bool Box::pointInside(const Vector3& point) const noexcept{
    Vector3 relative = point - pos;
    relative = orientation.rotate(relative);
    return relative.x < dimensions.x / 2 && relative.x > -dimensions.x / 2
        && relative.y < dimensions.y / 2 && relative.y > -dimensions.y / 2
        && relative.z < dimensions.z / 2 && relative.z > -dimensions.z / 2;
}

ld Cylinder::surfaceArea() const noexcept
{
    return 2 * PI * radius * (height + radius);
}
Vector<Vector3> Cylinder::pointApproximation(sizeT n = options::pointApproximationOfMeshesPerM2) const noexcept {
    Vector<Vector3> ans;
    
    ld area = surfaceArea();
    n *= area;
    sizeT sidePointAmount = (PI * 2 * radius * height * n) / area;
    sizeT topOrBottomPointAmount = (PI * radius * radius * n) / (area * 2);

    for (auto i = 0; i < sidePointAmount; i++) {

    }

    for (auto i = 0; i < topOrBottomPointAmount; i++) {

    }
}
bool Cylinder::pointInside(const Vector3& point) const noexcept {
    Vector3 relative = point - pos;
    relative = orientation.rotate(relative);
    return (relative.x > -height * 0.5 || relative.x < height * 0.5)
        && relative.y * relative.y + relative.z * relative.z < radius;
}

bool NoseCone::pointInside(const Vector3& point) const noexcept{
    Vector3 relative = point - pos;
    relative = orientation.rotate(relative);
    return (f(relative.y, relative.z) > relative.x && relative.x > 0);
}

bool collision(const Shape& shape, const Sphere& sphere) {
    Vector<Vector3> points = sphere.pointApproximation();
    return shape.pointsInside(points);
}
bool collision(const Shape& shape, const Box& box) {
    Vector<Vector3> points = box.pointApproximation();
    return shape.pointsInside(points);
}
bool collision(const Shape& shape, const Cylinder& cylinder) {
    Vector<Vector3> points = cylinder.pointApproximation();
    return shape.pointsInside(points);
}
bool collision(const Shape& shape, const NoseCone& noseCone) {
    Vector<Vector3> points = noseCone.pointApproximation();
    return shape.pointsInside(points);
}

bool collision(const Shape& s1, const Shape& s2) {
    for(const ShapeNode& node : s2.meshes)
        switch (node.meshType)
        {
        case MeshType::Sphere:
            if (collision(s1, node.sphere))
                return true;
        case MeshType::Box:
            if (collision(s1, node.box))
                return true;
        case MeshType::Cylinder:
            if (collision(s1, node.cylinder))
                return true;
        case MeshType::NoseCone:
            if (collision(s1, node.noseCone))
                return true;
        default:
            break;
        }
    for (const ShapeNode& node : s1.meshes)
        switch (node.meshType)
        {
        case MeshType::Sphere:
            if (collision(s2, node.sphere))
                return true;
        case MeshType::Box:
            if (collision(s2, node.box))
                return true;
        case MeshType::Cylinder:
            if (collision(s2, node.cylinder))
                return true;
        case MeshType::NoseCone:
            if (collision(s2, node.noseCone))
                return true;
        default:
            break;
        }
    return false;
}

bool Shape::pointInside(const Vector3& point) const noexcept {
    bool inside = false;
    for (const auto& i : meshes)
        switch (i.meshType)
        {
        case MeshType::Sphere:
            inside = i.sphere.pointInside(point);
        case MeshType::Box:
            inside = i.box.pointInside(point);
        case MeshType::Cylinder:
            inside = i.cylinder.pointInside(point);
        case MeshType::NoseCone:
            inside = i.noseCone.pointInside(point);
        default:
            break;
        }
    return inside;
}

bool Shape::pointsInside(const Vector<Vector3>& points) const noexcept {
    for (const auto& i : points)
        if (pointInside(i))
            return true;
    return false;
}


