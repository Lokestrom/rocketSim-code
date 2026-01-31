#include "../../gpuInterface/Model.hpp"

#include "../../helpers/math/Vector3.hpp"

namespace OpenGL
{
	class Vector
	{
	public:
		Vector(const glm::vec4& color);

		Vector(const Vector&) = delete;
		Vector& operator=(const Vector&) = delete;

		Vector(Vector&&) noexcept = default;
		Vector& operator=(Vector&&) noexcept = default;

		~Vector() = default;
		
		void Render(const Vector3& vec, const Vector3& position);
		
	private:
		GPU::Model3D model;
		glm::vec4 color;
	};

}