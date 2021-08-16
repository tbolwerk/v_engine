#include "app.hpp"

#include <stdexcept>

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
    void App::createCommandBuffer(){};
    void App::drawFrame(){};
}