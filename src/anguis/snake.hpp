#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/camera.hpp"
#include "../engine/window.hpp"
#include "../engine/model.hpp"

#include <vector>

class Snake {
    public:
        Snake();

        void render(Shader shader);
        void update(Window& window, float dt, Camera& camera);
        bool collide(glm::vec3 other, float radius);
        void grow();
        void die();

    private:
        Model m_model;
        Texture m_texture_head;
        Texture m_texture_body;

        std::vector<glm::vec2> positions;

        void moveTo(float x, float y);
        void join();
        void slither(float x, float y);

        float speed;
        float segmentSize;

        [[maybe_unused]] float dx, dy;
        glm::vec2 position; // ultimate position of snake (head)
};