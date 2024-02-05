#include "Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <unordered_map>

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
    seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
};

namespace std {
    template <>
    struct hash<Model3D::Vertex> {
        size_t operator()(Model3D::Vertex const& vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
    template <>
    struct hash<Model2D::Vertex> {
        size_t operator()(Model2D::Vertex const& vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color);
            return seed;
        }
    };
}  // namespace std

Model2D::Model2D(Device& device, const Model2D::Builder& builder) : _device{ device } {
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Model2D::~Model2D() {}

std::unique_ptr<Model2D> Model2D::createModelFromFile(Device& device, const std::string& filepath)
{
    Builder builder{};
    builder.loadModel(filepath);
    return std::make_unique<Model2D>(device, builder);
}

std::unique_ptr<Model2D> Model2D::createModelFromVertices(Device& device, const std::vector<Vertex>& vertices) {
    Builder builder{};
    builder.vertices = vertices;
    builder.indices.reserve(vertices.size());
    for (uint32_t i = 0; i < vertices.size(); i++)
        builder.indices.push_back(i);
    
    return std::make_unique<Model2D>(device, builder);
}

void Model2D::bind(vk::CommandBuffer commandBuffer)
{
    vk::Buffer buffers[] = { _vertexBuffer->getBuffer() };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);

    if (_hasIndexBuffer) {
        commandBuffer.bindIndexBuffer(_indexBuffer->getBuffer(), 0, vk::IndexType::eUint32);
    }
}

void Model2D::draw(vk::CommandBuffer commandBuffer)
{
    if (_hasIndexBuffer) {
        commandBuffer.drawIndexed(_indexCount, 1, 0, 0, 0);
    }
    else {
        commandBuffer.draw(_vertexCount, 1, 0, 0);
    }
}

void Model2D::createVertexBuffers(const std::vector<Vertex>& vertices)
{
    _vertexCount = static_cast<uint32_t>(vertices.size());
    assert(_vertexCount >= 3 && "Vertex count must be at least 3");
    vk::DeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer{
        _device,
        vertexSize,
        _vertexCount,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    _vertexBuffer = std::make_unique<Buffer>(
        _device,
        vertexSize,
        _vertexCount,
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);
}

void Model2D::createIndexBuffers(const std::vector<uint32_t>& indices)
{
    _indexCount = static_cast<uint32_t>(indices.size());
    _hasIndexBuffer = _indexCount > 0;

    if (!_hasIndexBuffer) {
        return;
    }

    vk::DeviceSize bufferSize = sizeof(indices[0]) * _indexCount;
    uint32_t indexSize = sizeof(indices[0]);

    Buffer stagingBuffer{
        _device,
        indexSize,
        _indexCount,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    _indexBuffer = std::make_unique<Buffer>(
        _device,
        indexSize,
        _indexCount,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffer.getBuffer(), _indexBuffer->getBuffer(), bufferSize);
}

std::vector<vk::VertexInputBindingDescription> Model2D::Vertex::getBindingDescriptions() {
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return bindingDescriptions;
}

std::vector<vk::VertexInputAttributeDescription> Model2D::Vertex::getAttributeDescriptions() {
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ 0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position) });
    attributeDescriptions.push_back({ 1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color) });

    return attributeDescriptions;
}

void Model2D::Builder::loadModel(const std::string& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
         std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            if (index.vertex_index >= 0) {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1]
                };

                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                    1
                };
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

Model3D::Model3D(Device& device, const Model3D::Builder& builder) : _device{ device } {
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Model3D::~Model3D() {}

std::unique_ptr<Model3D> Model3D::createModelFromFile(
    Device& device, const std::string& filepath) {
    Builder builder{};
    builder.loadModel(filepath);
    return std::make_unique<Model3D>(device, builder);
}

void Model3D::createVertexBuffers(const std::vector<Vertex>& vertices) {
    _vertexCount = static_cast<uint32_t>(vertices.size());
    assert(_vertexCount >= 3 && "Vertex count must be at least 3");
    vk::DeviceSize bufferSize = sizeof(vertices[0]) * _vertexCount;
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer{
        _device,
        vertexSize,
        _vertexCount,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    _vertexBuffer = std::make_unique<Buffer>(
        _device,
        vertexSize,
        _vertexCount,
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);
}

void Model3D::createIndexBuffers(const std::vector<uint32_t>& indices) {
    _indexCount = static_cast<uint32_t>(indices.size());
    _hasIndexBuffer = _indexCount > 0;

    if (!_hasIndexBuffer) {
        return;
    }

    vk::DeviceSize bufferSize = sizeof(indices[0]) * _indexCount;
    uint32_t indexSize = sizeof(indices[0]);

    Buffer stagingBuffer{
        _device,
        indexSize,
        _indexCount,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    _indexBuffer = std::make_unique<Buffer>(
        _device,
        indexSize,
        _indexCount,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffer.getBuffer(), _indexBuffer->getBuffer(), bufferSize);
}

void Model3D::draw(vk::CommandBuffer commandBuffer) {
    if (_hasIndexBuffer) {
        commandBuffer.drawIndexed(_indexCount, 1, 0, 0, 0);
    }
    else {
        commandBuffer.draw(_vertexCount, 1, 0, 0);
    }
}

void Model3D::bind(vk::CommandBuffer commandBuffer) {
    vk::Buffer buffers[] = { _vertexBuffer->getBuffer() };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);

    if (_hasIndexBuffer) {
        commandBuffer.bindIndexBuffer(_indexBuffer->getBuffer(), 0, vk::IndexType::eUint32);
    }
}

std::vector<vk::VertexInputBindingDescription> Model3D::Vertex::getBindingDescriptions() {
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return bindingDescriptions;
}

std::vector<vk::VertexInputAttributeDescription> Model3D::Vertex::getAttributeDescriptions() {
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) });
    attributeDescriptions.push_back({ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) });
    attributeDescriptions.push_back({ 2, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) });
    attributeDescriptions.push_back({ 3, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv) });

    return attributeDescriptions;
}

void Model3D::Builder::loadModel(const std::string& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
         std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

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
}
