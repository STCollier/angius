#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

class Model {
    public:
        Model(const std::string src);
        ~Model();

        void render(Shader shader, glm::mat4 model);

    private:
        std::vector<struct Vertex> vertices;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<unsigned int> positionIndices, uvIndices, normalIndices;

        unsigned int VBO, VAO;
        size_t vertexCount;
};