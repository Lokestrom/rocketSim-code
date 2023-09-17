#pragma once

#include "Buffer.hpp"
#include "Device.hpp"

#include "Vector.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

using namespace Database;

class Model2D
{
public:
    struct Vertex {
        glm::vec2 position{};
        glm::vec4 color{};

        static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string& filepath);
    };

    Model2D(Device& device, const Model2D::Builder& builder);
    ~Model2D();

    Model2D(const Model2D&) = delete;
    Model2D& operator=(const Model2D&) = delete;

    static std::unique_ptr<Model2D> createModelFromFile(
        Device& device, const std::string& filepath);
    static std::unique_ptr<Model2D> createModelFromVertices(
        Device& device, const std::vector<Vertex>& Vertices);

    void bind(vk::CommandBuffer commandBuffer);
    void draw(vk::CommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

private:
    Device& _device;

    std::unique_ptr<Buffer> _vertexBuffer;
    uint32_t _vertexCount;

    bool _hasIndexBuffer = false;
    std::unique_ptr<Buffer> _indexBuffer;
    uint32_t _indexCount;
};


class Model3D
{
public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color && normal == other.normal &&
                uv == other.uv;
        }
    };

    struct Builder {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string& filepath);
    };

    Model3D(Device& device, const Model3D::Builder& builder);
    ~Model3D();

    Model3D(const Model3D&) = delete;
    Model3D& operator=(const Model3D&) = delete;

    static std::unique_ptr<Model3D> createModelFromFile(
        Device& device, const std::string& filepath);

    void bind(vk::CommandBuffer commandBuffer);
    void draw(vk::CommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

private:
    Device& _device;

    std::unique_ptr<Buffer> _vertexBuffer;
    uint32_t _vertexCount;

    bool _hasIndexBuffer = false;
    std::unique_ptr<Buffer> _indexBuffer;
    uint32_t _indexCount;
};

