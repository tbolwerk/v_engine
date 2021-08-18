#pragma once

#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace v_engine
{
    struct TransformComponent
    {
        glm::vec3 translation{}; // position
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};
        glm::mat4 mat4()
        {
            auto transform = glm::translate(glm::mat4{1.f},translation);
            // Rotation convention used here is tait-bryan angles. with axis order Y(1) X(2) Z(3)
            transform = glm::rotate(transform, rotation.y, {0.f,1.f,0.f});
            transform = glm::rotate(transform, rotation.x, {1.f,0.f,0.f});
            transform = glm::rotate(transform, rotation.z, {0.f,0.f,1.f});

            transform = glm::scale(transform, scale);
            return transform;
        }
    };

    class GameObject
    {
    public:
        using id_t = unsigned int;
        static GameObject createGameObject()
        {
            static id_t currentId = 0;
            return GameObject{currentId++};
        }

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        const id_t getId() { return id; }

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        GameObject(id_t objId) : id{objId} {}
        id_t id;
    };
}