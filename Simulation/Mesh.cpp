#include "Mesh.hpp"
#include <algorithm>

bool collision(const Sphere& s1, const Sphere s2) {
	Vector3 relative = s1.pos - s2.pos;
	return relative.length() < s1.radius + s2.radius;
}

bool collision(const Sphere& sphere, const Box& box) {
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

bool collision(const Box& box1, const Box& box2) {
    // Calculate the distance between the centers of each box
    Vector3 distance = box2.pos - box1.pos;

    // Calculate the projection of the distance vector onto each axis of the first box
    Vector3 xAxis = box1.orientation * Vector3::UnitX();
    Vector3 yAxis = box1.orientation * Vector3::UnitY();
    Vector3 zAxis = box1.orientation * Vector3::UnitZ();
    ld xProjection = abs(distance.dot(xAxis));
    ld yProjection = abs(distance.dot(yAxis));
    ld zProjection = abs(distance.dot(zAxis));

    // Calculate the projection of the distance vector onto each axis of the second box
    Vector3 xAxis2 = box2.orientation * Vector3::UnitX();
    Vector3 yAxis2 = box2.orientation * Vector3::UnitY();
    Vector3 zAxis2 = box2.orientation * Vector3::UnitZ();
    ld xProjection2 = abs(distance.dot(xAxis2));
    ld yProjection2 = abs(distance.dot(yAxis2));
    ld zProjection2 = abs(distance.dot(zAxis2));

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
bool collision(Shape& shape, Sphere& sphere) {
    bool colliding = false;

    for (const auto& i : shape.meshes) {
        switch (i.meshType)
        {
        case 0:
            if (i.sphere.solid) {
                if (collision(i.sphere, sphere)) {
                    colliding = true;
                }
            }
            else
                if (engulfing(i.sphere, sphere))
                    colliding = false;
        case 1:
            if (i.box.solid) {
                if (collision(i.box, sphere)) {
                    colliding = true;
                }
            }
            else
                if (engulfing(i.box, sphere))
                    colliding = false;
        case 2:
            if (i.sylinder.solid) {
                if (collision(i.sylinder, sphere)) {
                    colliding = true;
                }
            }
            else
                if (engulfing(i.sylinder, sphere))
                    colliding = false;
        case 3:
            if (i.noseCone.solid) {
                if (collision(i.noseCone, sphere)) {
                    colliding = true;
                }
            }
            else
                if (engulfing(i.noseCone, sphere))
                    colliding = false;
        default:
            break;
        }
    }
}
bool collision(Shape& shape, Box& box) {
    for
}
bool collision(Shape& shape, Sylinder& sylinder) {
    for
}
bool collision(Shape& shape, NoseCone& noseCone) {
    for
}

bool collision(Shape s1, Shape s2) {
    for
}

bool pointInsideMesh(const Vector3& point, const Sphere& sphere) {
    Vector3 relative = point - sphere.pos;
    return relative.length() < sphere.radius && sphere.solid;
}

bool pointInsideMesh(const Vector3& point, const Box& box) {
    Vector3 relative = point - box.pos;
    relative = box.orientation.rotate(relative);
    return relative.x < box.dimensions.x / 2 && relative.x > -box.dimensions.x / 2
        && relative.y < box.dimensions.y / 2 && relative.y > -box.dimensions.y / 2
        && relative.z < box.dimensions.z / 2 && relative.z > -box.dimensions.z / 2
        && box.solid;
}

bool pointInsideMesh(const Vector3& point, const Sylinder& sylinder) {
    Vector3 relative = point - sylinder.pos;
    relative = sylinder.orientation.rotate(relative);
    return (relative.x > -sylinder.height * 0.5 || relative.x < sylinder.height * 0.5) 
        && relative.y * relative.y + relative.z * relative.z < sylinder.radius
        && sylinder.solid;
}

bool pointInsideMesh(const Vector3& point, const NoseCone& noseCone) {
    Vector3 relative = point - noseCone.pos;
    relative = noseCone.orientation.rotate(relative);
    return (noseCone.f(relative.y, relative.z) > relative.x && relative.x > 0) && noseCone.solid;
}

bool pointInsideShape(const Vector3& point, Shape& shape) {
    bool inside = false;
    for (const auto& i : shape.meshes)
        switch (i.meshType)
        {
        case 0:
            inside = pointInsideMesh(point, i.sphere);
        case 1:
            inside = pointInsideMesh(point, i.box);
        case 2:
            inside = pointInsideMesh(point, i.sylinder);
        case 3:
            inside = pointInsideMesh(point, i.noseCone);
        default:
            break;
        }
    return inside;
}
