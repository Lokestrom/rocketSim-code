#include "Shader.hpp"

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace OpenGL;

std::string getFileContents(const char* filename)
{
	std::ifstream file(filename);
	assert(file.is_open() && "Failed to open file");
	std::string content((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));
	return content;
}

bool CompileSuccessful(int obj) {
	int status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}

Shader::Shader(std::string vertex, std::string fragment)
{
	std::string vertexCode = getFileContents(vertex.c_str());
	std::string fragmentCode = getFileContents(fragment.c_str());

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	if (!CompileSuccessful(vertexShader))
		std::cerr << "Vertex shader compilation failed: " << vertex << std::endl;

	GLuint fragmentShader = 0;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	if (!CompileSuccessful(fragmentShader))
		std::cerr << "Fragment shader compilation failed:" << fragment << std::endl;

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);

	glLinkProgram(_shaderProgram);

	glUseProgram(_shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint linked = 0;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint logLength = 0;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetProgramInfoLog(_shaderProgram, logLength, nullptr, log.data());
		std::cerr << "Shader link error: " << log.data() << std::endl;
	}
}

Shader::~Shader()
{
	glDeleteProgram(_shaderProgram);
}

void Shader::bind() const 
{ 
	glUseProgram(_shaderProgram); 
}