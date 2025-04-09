#pragma once

#include "../engine/shader.hpp"
#include "window.hpp"

class Camera {
    public:
        Camera(Window& window);
        
        glm::mat4 projection;
        glm::mat4 view;

        void setMatrices(Shader& shader);
};