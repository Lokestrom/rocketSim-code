#include "Mesh.hpp"
#include <algorithm>

bool collision(Sphere& s1, Sphere s2) {
	Vector3 relative = s1.pos - s2.pos;
	return relative.length() < s1.radius + s2.radius;
}

bool collision(Sphere& sphere, Box& box) {
	// Transform the sphere's center position into the box's local space
	Vector3 spherePos = sphere.pos - box.pos;
	spherePos = box.orientation.inverse() * spherePos;

	// Find the closest point on the box to the sphere's center
	Vector3 closestPoint = spherePos;
	closestPoint.x = std::clamp(closestPoint.x, -box.dimensions.x / 2, box.dimensions.x / 2);
	closestPoint.y = std::clamp(closestPoint.y, -box.dimensions.y / 2, box.dimensions.y / 2);
	closestPoint.z = std::clamp(closestPoint.z, -box.dimensions.z / 2, box.dimensions.z / 2);

	// Calculate the distance between the closest point and the sphere's center
	ld distance = (closestPoint - spherePos).length();

	// Check if the distance is smaller than the sphere's radius
	return distance <= sphere.radius;
}

bool collision(Box& box1, Box& box2) {
    // Calculate the distance between the centers of each box
    Vector3 distance = box2.pos - box1.pos;

    // Calculate the projection of the distance vector onto each axis of the first box
    Vector3 xAxis = box1.orientation * Vector3::UnitX();
    Vector3 yAxis = box1.orientation * Vector3::UnitY();
    Vector3 zAxis = box1.orientation * Vector3::UnitZ();
    ld xProjection = std::abs(distance.dot(xAxis));
    ld yProjection = std::abs(distance.dot(yAxis));
    ld zProjection = std::abs(distance.dot(zAxis));

    // Calculate the projection of the distance vector onto each axis of the second box
    Vector3 xAxis2 = box2.orientation * Vector3::UnitX();
    Vector3 yAxis2 = box2.orientation * Vector3::UnitY();
    Vector3 zAxis2 = box2.orientation * Vector3::UnitZ();
    ld xProjection2 = std::abs(distance.dot(xAxis2));
    ld yProjection2 = std::abs(distance.dot(yAxis2));
    ld zProjection2 = std::abs(distance.dot(zAxis2));

    // Calculate the rotated dimensions of each box
    Vector3 box1Dimensions = box1.orientation * box1.dimensions;
    Vector3 box2Dimensions = box2.orientation * box2.dimensions;

    // Check if there is overlap on all axes
    return xProjection < box1Dimensions.x / 2 + box2Dimensions.x / 2 &&
        xProjection2 < box1Dimensions.x / 2 + box2Dimensions.x / 2 &&
        yProjection < box1Dimensions.y / 2 + box2Dimensions.y / 2 &&
        yProjection2 < box1Dimensions.y / 2 + box2Dimensions.y / 2 &&
        zProjection < box1Dimensions.z / 2 + box2Dimensions.z / 2 &&
        zProjection2 < box1Dimensions.z / 2 + box2Dimensions.z / 2;
}

bool collision(const Sphere& sphere, const NoseCone& noseCone) {
    // Transform the coordinates of the center of the sphere into a coordinate system where the nose cone is oriented along the z-axis
    Vector3 transformedPos = noseCone.orientation.rotate(sphere.pos - noseCone.pos);


    
}

bool pointInsideMesh(const Vector3& point, const Sphere& sphere) {
    Vector3 relative = point - sphere.pos;
    return relative.length() < sphere.radius;
}

bool pointInsideMesh(const Vector3& point, Box& noseCone) {
    return false;
}

bool pointInsideMesh(const Vector3& point, const Sylinder& sylinder) {
    Vector3 relative = point - sylinder.pos;
    relative = sylinder.orientation.rotate(relative);
    return (relative.x > -sylinder.height * 0.5 || relative.x < sylinder.height * 0.5) && relative.y * relative.y + relative.z * relative.z < sylider.radius;
}

bool pointInsideMesh(const Vector3& point, NoseCone& noseCone) {
    Vector3 relative = point - noseCone.pos;
    relative = noseCone.orientation.rotate(relative);
    return (noseCone.f(relative.y, relative.z) > relative.x && relative.x > 0);
}

bool pointInsideShape(const Vector3& point, Shape& shape) {
    for (auto i : shape.spheres)
        if (pointInsideMesh(point, i))
            return true;
    for (auto i : shape.boxes)
        if (pointInsideMesh(point, i))
            return true;
    for (auto i : shape.sylinders)
        if (pointInsideMesh(point, i))
            return true;
    for (auto i : shape.nosecones)
        if (pointInsideMesh(point, i))
            return true;

}
