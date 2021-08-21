#pragma once

#include "window.hpp"
#include "renderer.hpp"
#include "game_object.hpp"

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
        Window window{WIDTH, HEIGHT, "V_ENGINE"};
        Device device{window};
        std::vector<GameObject> gameObjects;
        Renderer renderer{window, device};

        void loadGameObjects();
    };
}