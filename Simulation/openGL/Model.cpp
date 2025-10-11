#include "Model.hpp"

#include "glad/glad.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "../FileSystem/external/tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

using namespace OpenGL;

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

namespace std {
	template <>
	struct hash<Model::Vertex> {
		size_t operator()(Model::Vertex const& vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}  // namespace std

Model::Model()
	: _size(0), _vertexArray(0), _indexBuffer(0)
{
}

Model::Model(std::filesystem::path filepath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.string().c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2],
				};
			}

			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
	_size = indices.size();
	createBuffers(vertices, indices);
}

Model::Model(std::vector<Model::Vertex> vertices)
	: _size(vertices.size())
{
	auto indices = createIndices(vertices);
	createBuffers(vertices, indices);
}

Model::~Model()
{
#ifdef OpenGLRendering
	if (_size) {
		glDeleteVertexArrays(1, &_vertexArray);
		glDeleteBuffers(1, &_indexBuffer);
	}
#endif
}

Model& Model::operator=(Model&& other) noexcept
{
	_size = other._size;
	_vertexArray = other._vertexArray;
	_indexBuffer = other._indexBuffer;
	other._size = 0;

	return *this;
}

void Model::render() const
{
#ifdef OpenGLRendering
	glBindVertexArray(_vertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, nullptr);
#endif
}

std::vector<unsigned int> Model::createIndices(std::vector<Vertex>& vertices)
{
	std::vector<unsigned int> indices;
	size_t uniqueVertexCount = 0;
	std::unordered_map<Vertex, unsigned int> uniqueVertices{};
	for (const auto& vertex : vertices) {
		if (uniqueVertices.count(vertex) == 0) {
			uniqueVertices[vertex] = static_cast<unsigned int>(uniqueVertices.size());
			vertices[uniqueVertexCount++] = vertex;
		}
		indices.push_back(uniqueVertices[vertex]);
	}
	vertices.resize(uniqueVertexCount);
	return indices;
}

void Model::createBuffers(std::vector<Model::Vertex> vertices, std::vector<unsigned int> indices)
{
#ifdef OpenGLRendering
	unsigned int VBO;
	glGenVertexArrays(1, &_vertexArray);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &_indexBuffer);

	glBindVertexArray(_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)offsetof(Model::Vertex, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
#endif
}