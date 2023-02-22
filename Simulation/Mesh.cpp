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

void applyCollisionForce(Sphere& sphere1, Sphere& sphere2)
{
    // Calculate the distance between the two sphere's centers
    Vector3 diff = sphere1.pos - sphere2.pos;
    ld distance = diff.length();

    // Check if the spheres are colliding
    if (distance < sphere1.radius + sphere2.radius)
    {
        // Calculate the collision normal vector (the direction in which the force will be applied)
        Vector3 normal = diff.normal();

        // Calculate the relative velocity of the spheres
        Vector3 rv = sphere1.vel - sphere2.vel;

        // Calculate the coefficient of restitution (how much energy is lost in the collision)
        ld COR = std::min(sphere1.restitution, sphere2.restitution);

        // Calculate the impulse scalar (the magnitude of the force to apply)
        ld j = -(1 + COR) * rv.dot(normal) / (normal.dot(normal) * (1 / sphere1.mass + 1 / sphere2.mass));

        // Calculate the collision impulse (the force to apply)
        Vector3 impulse = j * normal;

        // Apply the impulse to each sphere
        sphere1.vel += impulse / sphere1.mass;
        sphere2.vel -= impulse / sphere2.mass;
    }
}

