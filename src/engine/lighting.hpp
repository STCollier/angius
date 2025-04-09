#pragma once

#include "shader.hpp"

class Lighting {
    public:
        Lighting();

        void shadowPass(Shader shader);
        unsigned int getDepthMapFBO() { return depthMapFBO; }
        unsigned int getDepthMap()    { return depthMap; }

        float shadowResolution;
        glm::mat4 lightSpaceMatrix;
        glm::vec3 lightPos;

    private:
        unsigned int depthMapFBO, depthMap;
};