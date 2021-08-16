#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace v_engine {

    class Window {
        private:
            const int width;
            const int height;
            std::string windowName;
            GLFWwindow *window;

            void initWindow();
        public:
            Window(int w, int h, std::string name);
            ~Window();

            Window(const Window &) = delete;
            Window &operator=(const Window &) = delete;
            
            bool shouldClose(){ return glfwWindowShouldClose(window); }
    };
}