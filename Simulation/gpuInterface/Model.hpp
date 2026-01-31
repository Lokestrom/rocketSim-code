#pragma once

#include "glm/glm.hpp"
#include "FileSystem/external/tiny_obj_loader.h"

#include <filesystem>
#include <vector>
#include <unordered_map>

namespace GPU {
	class Model3D;

	namespace Model {
		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;

			bool operator==(const Vertex& other) const {
				return position == other.position && 
					color == other.color;
			}
		};

		Model3D LoadFromFile(std::filesystem::path filepath);
	}

#ifdef OpenGLRendering
	class Model3D {
		friend class RenderingModel3D;
	public:
		Model3D() = delete;
		Model3D(const std::vector<Model::Vertex>& vertices, const std::vector<unsigned int>& indices);
		Model3D(Model3D&) = delete;
		Model3D(Model3D&&) noexcept;
		~Model3D();

		Model3D& operator=(Model3D&) = delete;
		Model3D& operator=(Model3D&& model) noexcept;

		unsigned int getSize() const { return _size; }

	private:
		void createBuffer(const std::vector<Model::Vertex>& vertices, const std::vector<unsigned int>& indices);
	private:
		unsigned int _size;
		unsigned int _vertexArray;
		unsigned int _indexBuffer;
		unsigned int _vertexBuffer;
	};

	class RenderingModel3D {
	public:
		RenderingModel3D(Model3D& model);
		~RenderingModel3D();

		void render();

		void bindBuffers(Model3D& model);
	private:
		unsigned int _vertexArray;
		unsigned int _size;
	};
#endif
}