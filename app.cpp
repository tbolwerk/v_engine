#include "app.hpp"

#include <stdexcept>
#include <array>
#include <vector>

namespace v_engine
{
    App::App()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffer();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void App::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device.device());
    }

    void sierpinski(
        std::vector<Model::Vertex> &vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top)
    {
        if (depth <= 0)
        {
            vertices.push_back({top});
            vertices.push_back({right});
            vertices.push_back({left});
        }
        else
        {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

    void App::loadModels()
    {
        // std::vector<Model::Vertex> vertices{};
        // sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        model = std::make_unique<Model>(device, vertices);
    }

    void App::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Pipeline layout creation failed");
        }
    }
    void App::createPipeline()
    {
        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = swapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device, pipelineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");
    }
    void App::createCommandBuffer()
    {
        commandBuffers.resize(swapChain->imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to allocatie command buffers");
        }
    };

    void App::freeCommandBuffers()
    {
        vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    void App::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to begin recording command buffers");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {0.1f, 0}; //TODO 0 = 0.0f ??
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        pipeline->bind(commandBuffers[imageIndex]);
        model->bind(commandBuffers[imageIndex]);
        model->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to record command buffer");
        }
    }

    void App::recreateSwapChain()
    {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = window.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.device());
        // swapChain = nullptr; // Fixes issues on other devices..
        if (swapChain == nullptr)
        {
            swapChain = std::make_unique<SwapChain>(device, extent);
        }
        else
        {
            swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
            if(swapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        // If render pass is compatible do nothing
        createPipeline();
    }

    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("ERROR: Failed to acquire swap chain image, or screen resized");
        }

        recordCommandBuffer(imageIndex);
        result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
        {
            window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to present swap chain image");
        }
    };
}