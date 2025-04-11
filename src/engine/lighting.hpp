#pragma once

#include "shader.hpp"
#include "camera.hpp"

class Lighting {
    public:
        Lighting();

        void shadowPass(Shader shader, Camera& camera);
        unsigned int getDepthMapFBO() { return depthMapFBO; }
        unsigned int getDepthMap()    { return depthMap; }

        float shadowResolution;
        glm::mat4 lightSpaceMatrix;
        glm::vec3 lightPos;

    private:
        unsigned int depthMapFBO, depthMap;
};