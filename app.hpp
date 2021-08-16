#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "model.hpp"

#include <memory>
#include <vector>
namespace v_engine
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();

    private:
        Window window{WIDTH, HEIGHT, "Hello vulkan!"};
        Device device{window};
        SwapChain swapChain{device, window.getExtent()};
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffer();
        void drawFrame();
        void loadModels();
    };
}