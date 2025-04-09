#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(Window& window) {
    view = glm::mat4(1.0f);
    projection  = glm::mat4(1.0f);

    view = glm::rotate(view, (float) glm::radians(45.0), glm::vec3(1.f, 0.f, 0.f));
    view = glm::translate(view, glm::vec3(0.0f, -100.0f, -100.0f)); // z component

    // TODO: make sure lighting uses identical near and far plane values
    projection = glm::perspective(glm::radians(45.0f), (float) window.width / (float) window.height, 0.1f, 1000.0f);
}

void Camera::setMatrices(Shader& shader) {
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}