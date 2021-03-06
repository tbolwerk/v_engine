#pragma once

#include "game_object.hpp"
#include "window.hpp"

namespace v_engine {
    class MovementController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        struct MousePosition {
            double x;
            double y;
        };

        void moveInPlaneXZ(GLFWwindow *window, float dt, GameObject &gameObject) const;
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void init(GLFWwindow *window);
        KeyMappings keys{};
        float moveSpeed{3.f};
        float keyboardSensitivity{1.5f};
        float mouseSensitivity{5.5f};
    };
}