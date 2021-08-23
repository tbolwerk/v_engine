#include "movement_controller.hpp"

#include <limits>
#include <iostream>

namespace v_engine {
    MovementController::MousePosition mousePosition{};

    void MovementController::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            mousePosition.x = xPos;
            mousePosition.y = yPos;
        }
    }

    void MovementController::init(GLFWwindow *window) {
        mousePosition.x = 0.f;
        mousePosition.y = 0.f;
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    }

    void MovementController::moveInPlaneXZ(GLFWwindow *window, float dt, GameObject &gameObject) const {
        glm::vec3 rotate{0};

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);

            rotate.y -= static_cast<float>(mousePosition.x - xPos);
            rotate.x -= static_cast<float>(mousePosition.y - yPos);
        } else {
            if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) {
                rotate.y += 1.f;
            }
            if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) {
                rotate.y -= 1.f;
            }
            if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) {
                rotate.x += 1.f;
            }
            if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) {
                rotate.x -= 1.f;
            }
        }
        if (glm::dot(rotate, rotate) >
            std::numeric_limits<float>::epsilon()) // check if not 0, normalize will fail because of div by 0
        {
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y,
                                                   glm::two_pi<float>()); // make sure value in y is not overflowing, since its growing.

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        glm::vec3 moveDir{0.f};
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) {
            moveDir += forwardDir;
        }
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) {
            moveDir -= forwardDir;
        }
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) {
            moveDir += rightDir;
        }
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) {
            moveDir -= rightDir;
        }
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) {
            moveDir += upDir;
        }
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) {
            moveDir -= upDir;
        }
        if (glm::dot(moveDir, moveDir) >
            std::numeric_limits<float>::epsilon()) // check if not 0, normalize will fail because of div by 0
        {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }
    }


}