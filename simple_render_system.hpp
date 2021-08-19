#pragma once

#include "camera.hpp"
#include "pipeline.hpp"
#include "game_object.hpp"

#include <memory>
#include <vector>
namespace v_engine
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects,const Camera &camera);

    private:
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        
        Device &device;

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
    };
}