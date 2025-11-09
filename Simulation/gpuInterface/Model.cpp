#include "Model.hpp"

#include "glad/glad.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <iostream>

using namespace GPU;

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
			hashCombine(seed, vertex.position);
			hashCombine(seed, vertex.color);
			return seed;
		}
	};
}

Model3D Model::LoadFromFile(std::filesystem::path filepath)
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

	int orginalVertexCount = 0;

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			orginalVertexCount++;
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

			//			if (index.normal_index >= 0) {
			//				vertex.normal = {
			//					attrib.normals[3 * index.normal_index + 0],
			//					attrib.normals[3 * index.normal_index + 1],
			//					attrib.normals[3 * index.normal_index + 2],
			//				};
			//			}
			//
			//			if (index.texcoord_index >= 0) {
			//				vertex.uv = {
			//					attrib.texcoords[2 * index.texcoord_index + 0],
			//					attrib.texcoords[2 * index.texcoord_index + 1],
			//				};
			//			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}

	return { vertices, indices };
}

#ifdef OpenGLRendering

Model3D::Model3D(const std::vector<Model::Vertex>& vertices, const std::vector<unsigned int>& indices) {
	_size = indices.size();
	createBuffer(vertices, indices);
}

Model3D::~Model3D()
{
	glDeleteVertexArrays(1, &_vertexArray);
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_indexBuffer);
}

Model3D& Model3D::operator=(Model3D&& model) noexcept {
	_size = model._size;
	_vertexArray = model._vertexArray;
	_indexBuffer = model._indexBuffer;

	model._size = 0;
	model._vertexArray = 0;
	model._indexBuffer = 0;
	return *this;
}

void Model3D::createBuffer(const std::vector<Model::Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	glGenVertexArrays(1, &_vertexArray);
	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_indexBuffer);

	glBindVertexArray(_vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

RenderingModel3D::RenderingModel3D(Model3D& model)
	: _size(model._size)
{
	bindBuffers(model);
}

RenderingModel3D::~RenderingModel3D()
{
	glDeleteVertexArrays(1, &_vertexArray);
}

void RenderingModel3D::render() {
	glBindVertexArray(_vertexArray);
	glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, nullptr);
}

void RenderingModel3D::bindBuffers(Model3D& model)
{
	glGenVertexArrays(1, &_vertexArray);
	glBindVertexArray(_vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, model._vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model._indexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)offsetof(Model::Vertex, color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#endif