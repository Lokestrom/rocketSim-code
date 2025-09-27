#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

namespace OpenGL {
    class Model
    {
    public:
        //using ID = size_t;
        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            bool operator==(const Vertex& other) const {
                return position == other.position && color == other.color && normal == other.normal &&
                    uv == other.uv;
            }
        };

        Model();
		Model(std::filesystem::path filepath);
        Model(std::vector<Model::Vertex> vertecies);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

		Model(Model&&) = default;
		Model& operator=(Model&&) noexcept;

		void render() const;

    private:
        unsigned int _vertexArray;
        unsigned int _indexBuffer;
        unsigned int _size;

    private:
		std::vector<unsigned int> createIndices(std::vector<Vertex>& vertecies);
        void createBuffers(std::vector<Model::Vertex> vertecies, std::vector<unsigned int> indecies);
    };
}
