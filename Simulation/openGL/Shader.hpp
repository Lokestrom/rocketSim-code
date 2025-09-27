#pragma once

#include <string>

namespace OpenGL
{
	class Shader
	{
	public:
		Shader(std::string vertex, std::string fragment);
		~Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void bind() const;

	private:
		unsigned int _shaderProgram;
	};
}

