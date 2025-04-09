#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../engine/util.hpp"
#include "snake.hpp"

Snake::Snake() : m_texture("res/textures/snake.png", NEAREST), speed{32.0f}, segmentSize{1.0f}, dx{0.0f}, dy{0.0f}, pos{0.0f, 0.0f} {
    for (int i = 5; i >= 0; i--) {
        positions.push_back(glm::vec2(0.0f, static_cast<float>(i)));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(util::DEFAULT_CUBE_VERTICIES), util::DEFAULT_CUBE_VERTICIES, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(util::DEFAULT_CUBE_INDICIES), util::DEFAULT_CUBE_INDICIES, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   

    // norm
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);   

}

Snake::~Snake() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Snake::update(Window& window, float dt) {
    if (window.keyDown(GLFW_KEY_W)) dy -= 1.0f;
    if (window.keyDown(GLFW_KEY_S)) dy += 1.0f;
    if (window.keyDown(GLFW_KEY_A)) dx -= 1.0f;
    if (window.keyDown(GLFW_KEY_D)) dx += 1.0f;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length != 0.0f) {
        dx /= length;
        dy /= length;
    }

    pos.x += dx * speed * dt;
    pos.y += dy * speed * dt;

    slither(pos.x, pos.y);

    if (window.keyDown(GLFW_KEY_SPACE)) {
        grow();
    }
}


void Snake::render(Shader shader) {
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();
    
    glBindVertexArray(VAO);

    for (size_t i = 0; i < positions.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f));
        model = glm::translate(model, glm::vec3(positions[i].x, 2.0f, positions[i].y));

        shader.setMat4("model", model);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Snake::moveTo(float x, float y) {
    positions[0].x = x;
    positions[0].y = y;

    join();
}

void Snake::join() {
    for (size_t i = 1; i < positions.size(); i++) {
        glm::vec2 last = positions[i - 1];
        glm::vec2& curr = positions[i];
    
        // get difference in x and y of each position
        float _dx = curr.x - last.x;
        float _dy = curr.y - last.y;
    
        // calculate the angle between the two parts of the snake
        float angle = atan2f(_dy, _dx);
    
        // get the new x and new y using polar coordinates
        float nx = 2 * segmentSize * cosf(angle);
        float ny = 2 * segmentSize * sinf(angle);
    
        // add the new x and new y to the last snake's position to "join" the two together without a gap
        curr.x = nx + last.x;
        curr.y = ny + last.y;
    }
}

void Snake::slither(float x, float y) {
    glm::vec2 head = positions[0];
    
    // smooth movement
    float nx = head.x + (x - head.x) / 10;
    float ny = head.y + (y - head.y) / 10;
    
    moveTo(nx, ny);
}

void Snake::grow() {
    glm::vec2 tail = positions.back();

    glm::vec2 penultimateTail = positions[positions.size() - 2];
    float dx = tail.x - penultimateTail.x;
    float dy = tail.y - penultimateTail.y;


    glm::vec2 newSegment = tail + glm::vec2(dx, dy);
    positions.push_back(newSegment);
}