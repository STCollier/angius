#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../engine/util.hpp"
#include "ground.hpp"


Ground::Ground() : m_texture("res/textures/ground128.png", NEAREST) {
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

Ground::~Ground() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Ground::render(Shader shader) {
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();
    
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(128.0f, 1.f, 128.0f));

    shader.setMat4("model", model);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}