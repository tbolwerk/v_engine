#include "app.hpp"

#include <stdexcept>
#include <array>
namespace v_engine
{
    App::App()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffer();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(device.device(),pipelineLayout,nullptr);
    }

    void App::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }
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
        auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
        pipelineConfig.renderPass = swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device, pipelineConfig, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv");
    }
    void App::createCommandBuffer()
    {
        commandBuffers.resize(swapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to allocatie command buffers");
        }

        for(int i = 0; i < commandBuffers.size(); i ++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("ERROR: Failed to begin recording command buffers");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain.getRenderPass();
            renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {0.1f,0}; //TODO 0 = 0.0f ??
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i],3,1,0,0);
            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("ERROR: Failed to record command buffer");
            }
        }
    };
    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("ERROR: Failed to acquire swap chain image, or screen resized");
        }

        result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if(result != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to present swap chain image");
        }
    };
}