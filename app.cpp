#include "app.hpp"

namespace v_engine
{
    void App::run()
    {
        while(!window.shouldClose())
        {
            glfwPollEvents();
        }
    }
}