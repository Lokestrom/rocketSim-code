#include "vector.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

OpenGL::Vector::Vector(const glm::vec4& color)
	: model(GPU::Model::LoadFromFile(ASSET_DIR "VectorArrow.obj")),
	color(color)
{
}

glm::mat4 CreateTransformationMatrix(const Vector3& vec, const Vector3& position) {
    glm::mat4 matrix(1.0f);

    Vector3 dir = vec.normal();

    glm::quat rot = glm::rotation(
        glm::vec3(0.0f, 1.0f, 0.0f),   // model axis
        glm::vec3(
            static_cast<float>(dir.x),
            static_cast<float>(dir.y),
            static_cast<float>(dir.z)
        )
    );

    matrix = glm::translate(matrix, glm::vec3(
        static_cast<float>(position.x),
        static_cast<float>(position.y),
        static_cast<float>(position.z)
    ));

    matrix *= glm::mat4_cast(rot);

    matrix = glm::scale(matrix, glm::vec3(
        1.0f,
        static_cast<float>(vec.length()),
        1.0f
    ));

    return matrix;
}

void OpenGL::Vector::Render(const Vector3& vec, const Vector3& position)
{
	if (vec.lengthSquared() < 1e-12) {
        return;
    }
	glUniformMatrix4fv(2, 1, GL_FALSE, &CreateTransformationMatrix(vec, position)[0][0]);
	glUniform4f(3, color.r, color.g, color.b, color.a);
	GPU::RenderingModel3D(model).render();
}
