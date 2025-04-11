#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// very specific near and far planes to prevent shadow aliasing
Camera::Camera(Window& window) : near{0.1f}, far{10000.0f}, pitch{0.0f}, yaw{-90.0f}, FOV{45.0f}, sensitivity{0.1f} {
    lastX = window.width / 2.0f;
    lastY = window.height / 2.0f;
    
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    view = glm::rotate(view, (float) glm::radians(45.0), glm::vec3(1.f, 0.f, 0.f));
    view = glm::translate(view, glm::vec3(0.0f, -100.0f, -100.0f));

    projection = glm::perspective(glm::radians(FOV), (float) window.width / (float) window.height, near, far);
}

void Camera::setMatrices(Shader& shader) {
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

void Camera::update(Window& window) {
    float xpos = static_cast<float>(window.mouseX);
    float ypos = static_cast<float>(window.mouseY);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(front);
}