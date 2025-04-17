#include "food.hpp"

#include "../engine/util.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

static std::vector<glm::vec3> colors = {
    glm::vec3(0, 1.0, 1.0),
    glm::vec3(0.5, 1.0, 1.0),
    glm::vec3(1.0),
};

Food::Food() : m_model("res/models/icosphere.obj"), m_texture("res/textures/blank.png", LINEAR) {
    for (int i = 0; i < 64; i++) {
        spawnPellet();
    }
}

void Food::spawnPellet() {
    float radius = 100.0f;
    glm::vec2 center = glm::vec2(0.0, 0.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_int_distribution<std::size_t> listDist(0, colors.size() - 1);

    float r = radius * sqrt(dist(gen));
    float theta = dist(gen) * util::TAU;

    float x = center.x + r * cosf(theta);
    float y = center.y + r * sinf(theta);

    foodPellets.push_back({
        glm::vec3(x, 0.0, y),
        colors[listDist(gen)],
        dist(gen)
    });
}

void Food::render(Shader shader, Snake& snake, float time) {
    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();

    for (auto it = foodPellets.begin(); it != foodPellets.end(); ) {
        glm::vec3 position = glm::vec3(it->position.x, 0.75f + sinf(time + it->randomOffset * 100.0f) / 2.0f, it->position.z);
        float size = 0.25f;
    
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(size));
    
        shader.setVec3("pelletColor", it->color * glm::vec3(1.5f));
        m_model.render(shader, model);
        
        float margin = 2.5f;
        if (snake.collide(position, size * margin)) {
            it = foodPellets.erase(it);
            spawnPellet();
            snake.grow();
        } else {
            it++;
        }
    }

    shader.setVec3("pelletColor", glm::vec3(1.0, 1.0, 1.0));
}