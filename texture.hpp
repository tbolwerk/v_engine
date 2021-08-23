#pragma once

//
// Created by Twan Bolwerk on 23/08/2021.
//

// libs
#include <vulkan/vulkan.h>

// std
#include <memory>
#include <string>

#include "device.hpp"

namespace v_engine {

    class Texture {
    public:
        Texture(Device &device) : device{device} {}

        ~Texture() {
            vkDestroySampler(device.device(), textureSampler, nullptr);
            vkDestroyImageView(device.device(), textureImageView, nullptr);
            vkDestroyImage(device.device(), textureImage, nullptr);
            vkFreeMemory(device.device(), textureImageMemory, nullptr);
        }

        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        static std::unique_ptr<Texture> loadTexture(Device &device, const std::string &textureFilePath);

        void updateDescriptor();

        VkDescriptorImageInfo descriptor;

        VkImageView imageView() const { return textureImageView; }

        VkSampler sampler() const { return textureSampler; }

    private:
        Device &device;


        void
        createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

        void createTextureImageView(VkImageViewType viewType);

        void createTextureSampler();

        void createTextureImage(const std::string &textureFilePath);


        VkDeviceMemory textureImageMemory;
        VkImage textureImage;

        VkImageView textureImageView;
        VkSampler textureSampler;

        VkImageLayout textureLayout{VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        uint32_t mipLevels{1};
        uint32_t layerCount{1};
    };
}