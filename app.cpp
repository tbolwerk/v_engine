#include "app.hpp"
#include "keyboard_movement_controller.hpp"
#include "camera.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>
#include <vector>
#include <chrono>

namespace v_engine
{

    App::App()
    {
        loadGameObjects();
    }

    App::~App()
    {
    }

    void App::run()
    {
        std::cout << "maxPushConstantSize = " << device.properties.limits.maxPushConstantsSize << std::endl;
        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        Camera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = GameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }



    void App::loadGameObjects()
    {
        std::shared_ptr<Model> coloredCubeModel = Model::createModelFromFile(device, "/Users/twanbolwerk/Documents/dev/game-engine/models/colored_cube.obj");

        auto coloredCube = GameObject::createGameObject();
        coloredCube.model = coloredCubeModel;
        coloredCube.transform.translation = {.0f, .0f, 2.5f};
        coloredCube.transform.scale = {.5f, .5f, .5f};

        gameObjects.push_back(std::move(coloredCube));

        std::shared_ptr<Model> smoothVaseModel = Model::createModelFromFile(device, "/Users/twanbolwerk/Documents/dev/game-engine/models/smooth_vase.obj");

        auto smoothVase = GameObject::createGameObject();
        smoothVase.model = smoothVaseModel;
        smoothVase.transform.translation = {1.f, 1.f, 1.f};
        smoothVase.transform.scale = glm::vec3(3.f);

        gameObjects.push_back(std::move(smoothVase));
    }
}