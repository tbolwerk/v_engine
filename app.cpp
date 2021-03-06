#include "app.hpp"
#include "movement_controller.hpp"
#include "camera.hpp"
#include "simple_render_system.hpp"
#include "gui.hpp"
#include "texture.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

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

        // create imgui, and pass in dependencies
        GUI gui{
            window,
            device,
            renderer.getSwapChainRenderPass(),
            renderer.getImageCount()};

        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        Camera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = GameObject::createGameObject();
        MovementController cameraController{};
        MovementController::init(window.getGLFWwindow());

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
                gui.newFrame();
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                gui.runExample();
                gui.render(commandBuffer);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void App::loadGameObjects()
    {
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "models/colored_cube.obj");
        std::shared_ptr<Texture> texture = Texture::loadTexture(device, "textures/viking_room.png");

        auto coloredCube = GameObject::createGameObject();
        coloredCube.model = model;
        coloredCube.transform.translation = {1.5f, 0.2f, 2.5f};
        coloredCube.transform.scale = {.25f, .25f, .25f};
        gameObjects.push_back(std::move(coloredCube));

        model = Model::createModelFromFile(device, "models/cube.obj");
        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = {-1.5f, 0.2f, 2.5f};
        cube.transform.scale = {.25f, .25f, .25f};
        gameObjects.push_back(std::move(cube));

        model =
            Model::createModelFromFile(device, "models/flat_vase.obj");
        auto flatVase = GameObject::createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = {-.5f, .5f, 2.5f};
        flatVase.transform.scale = {3.f, 1.5f, 3.f};
        gameObjects.push_back(std::move(flatVase));

        model = Model::createModelFromFile(device, "models/smooth_vase.obj");
        auto smoothVase = GameObject::createGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = {.5f, .5f, 2.5f};
        smoothVase.transform.scale = {3.f, 1.5f, 3.f};
        gameObjects.push_back(std::move(smoothVase));

        model = Model::createModelFromFile(device, "models/donut.obj");
        auto donut = GameObject::createGameObject();
        donut.model = model;
        donut.transform.translation = {0.f, 1.f, 2.5f};
        donut.transform.scale = {5.f, 5.f, 5.f};
        gameObjects.push_back(std::move(donut));

        model = Model::createModelFromFile(device, "models/corona.obj");
        auto corona = GameObject::createGameObject();
        corona.model = model;
        corona.transform.translation = {-1.5f, 1.f, -.05f};
        corona.transform.scale = {.5f, .5f, .5f};
        gameObjects.push_back(std::move(corona));

        model = Model::createModelFromFile(device, "models/viking_room.obj");
        auto vikingRoom = GameObject::createGameObject();
        vikingRoom.model = model;
        vikingRoom.transform.translation = {1.5f, 1.f,-.05f};
        vikingRoom.transform.scale = {1.f,1.f,1.f};
        vikingRoom.transform.rotation = {1.5708f, 3.14f, 0.0f};
        gameObjects.push_back(std::move(vikingRoom));
    }
}