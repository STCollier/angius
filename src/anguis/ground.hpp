#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"

class Ground {
    public:
        Ground();
        ~Ground();

        void render(Shader shader);

    private:
        unsigned int VBO, VAO, EBO;
        Texture m_texture;
};