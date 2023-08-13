#pragma once

#include "Device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

class DescriptorSetLayout {
public:
    class Builder {
    public:
        Builder(Device& device) : _device{ device } {}

        Builder& addBinding(
            uint32_t binding,
            vk::DescriptorType descriptorType,
            vk::ShaderStageFlags stageFlags,
            uint32_t count = 1);
        std::unique_ptr<DescriptorSetLayout> build() const;

    private:
        Device& _device;
        std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> _bindings{};
    };

    DescriptorSetLayout(
        Device& lveDevice, std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindings);
    ~DescriptorSetLayout();
    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

    vk::DescriptorSetLayout getDescriptorSetLayout() const { return _descriptorSetLayout; }

private:
    Device& _device;
    vk::DescriptorSetLayout _descriptorSetLayout;
    std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> _bindings;

    friend class DescriptorWriter;
};

class DescriptorPool {
public:
    class Builder {
    public:
        Builder(Device& lveDevice) : _device{ lveDevice } {}

        Builder& addPoolSize(vk::DescriptorType descriptorType, uint32_t count);
        Builder& setPoolFlags(vk::DescriptorPoolCreateFlags flags);
        Builder& setMaxSets(uint32_t count);
        std::unique_ptr<DescriptorPool> build() const;

    private:
        Device& _device;
        std::vector<vk::DescriptorPoolSize> _poolSizes{};
        uint32_t _maxSets = 1000;
        vk::DescriptorPoolCreateFlags _poolFlags;
    };

    DescriptorPool(
        Device& lveDevice,
        uint32_t maxSets,
        vk::DescriptorPoolCreateFlags poolFlags,
        const std::vector<vk::DescriptorPoolSize>& poolSizes);
    ~DescriptorPool();
    DescriptorPool(const DescriptorPool&) = delete;
    DescriptorPool& operator=(const DescriptorPool&) = delete;

    bool allocateDescriptor(
        const vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorSet& descriptor) const;

    void freeDescriptors(std::vector<vk::DescriptorSet>& descriptors) const;

    void resetPool();

private:
    Device& _device;
    vk::DescriptorPool _descriptorPool;

    friend class DescriptorWriter;
};

class DescriptorWriter {
public:
    DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool);

    DescriptorWriter& writeBuffer(uint32_t binding, vk::DescriptorBufferInfo* bufferInfo);
    DescriptorWriter& writeImage(uint32_t binding, vk::DescriptorImageInfo* imageInfo);

    bool build(vk::DescriptorSet& set);
    void overwrite(vk::DescriptorSet& set);

private:
    DescriptorSetLayout& _setLayout;
    DescriptorPool& _pool;
    std::vector<vk::WriteDescriptorSet> _writes;
};
