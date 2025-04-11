#pragma once

#include "../engine/shader.hpp"
#include "window.hpp"

class Camera {
    public:
        Camera(Window& window);

        void setMatrices(Shader& shader);
        void update(Window& window);
        
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec3 forward;

        float near, far;

    private:
        float lastX, lastY, pitch, yaw, FOV, sensitivity;
};