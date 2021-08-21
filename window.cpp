#include "window.hpp"
#include <stdexcept>
#include <iostream>

namespace v_engine
{
    Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            std::cerr << "Failed to create window surface"<< std::endl;
            throw std::runtime_error("ERROR: Failed to create window surface");
        }
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto newWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        newWindow->framebufferResized = true;
        newWindow->width = width;
        newWindow->height = height;
    }
}