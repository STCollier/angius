#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/window.hpp"

#include <vector>

enum Direction {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};

class Snake {
    public:
        Snake();
        ~Snake();

        void render(Shader shader);
        void update(Window& window, float dt);

    private:
        unsigned int VBO, VAO, EBO;
        Texture m_texture;

        std::vector<glm::vec2> positions;

        void moveTo(float x, float y);
        void join();
        void slither(float x, float y);
        void grow();

        float speed;
        float segmentSize;

        float dx, dy;
        glm::vec2 pos;
};