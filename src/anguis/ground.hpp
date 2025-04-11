#pragma once

#include "../engine/texture.hpp"
#include "../engine/shader.hpp"
#include "../engine/model.hpp"

class Ground {
    public:
        Ground() : m_model("res/models/default_cube.obj"), m_texture("res/textures/ground128.png", NEAREST) {};

        void render(Shader shader);

    private:
        Model m_model;
        Texture m_texture;
};