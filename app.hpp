#pragma once

#include "window.hpp"
#include "pipeline.hpp"

namespace v_engine
{
    class App
    {
        public:
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            void run();
        private:
            Window window{WIDTH, HEIGHT,"Hello vulkan!"};
            Pipeline pipeline{"../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv"}; // TODO: Fix cmake so it compiles shaders aswell.
    };
}