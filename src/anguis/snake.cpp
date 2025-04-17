#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../engine/util.hpp"
#include "snake.hpp"

Snake::Snake() : m_model("res/models/snake.obj"), m_texture_head("res/textures/snake.hdr", NEAREST), m_texture_body("res/textures/snake_purple.hdr", NEAREST), speed{32.0f}, segmentSize{1.0f}, dx{0.0f}, dy{0.0f}, position{0.0f, 0.0f} {
    positions.push_back(glm::vec2(0.f, 0.f));
    
    for (int i = 1; i < 16; i++) {
        positions.push_back(glm::vec2(0.0f, static_cast<float>(i) * segmentSize * 2));
    }
}

bool firstPerson = false;
bool toggled = false;

void Snake::update(Window& window, float dt, Camera& camera) {
    
    // Third person movement with WASD
    /*if (window.keyDown(GLFW_KEY_W)) dy -= 1.0f;
    if (window.keyDown(GLFW_KEY_S)) dy += 1.0f;
    if (window.keyDown(GLFW_KEY_A)) dx -= 1.0f;
    if (window.keyDown(GLFW_KEY_D)) dx += 1.0f;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length != 0.0f) {
        dx /= length;
        dy /= length;
    }

    position.x += dx * speed * dt;
    position.y += dy * speed * dt;*/

    if (window.keyDown(GLFW_KEY_TAB) && !toggled) {
        firstPerson = !firstPerson;
        toggled = true;
    }

    if (!window.keyDown(GLFW_KEY_TAB) && toggled) toggled = false;

    glm::vec2 direction = glm::normalize(glm::vec2(camera.forward.x, camera.forward.z));
    position += direction * speed * dt;

    camera.view = glm::mat4(1.0f);

    if (firstPerson) {
        glm::vec3 camPos = glm::vec3(position.x, 2.0f, position.y);
        glm::vec3 direction3D = glm::normalize(glm::vec3(direction.x, 0.0f, direction.y));
        glm::vec3 camTarget = camPos + direction3D;
        
        camera.view = glm::lookAt(camPos, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        camera.position = camPos;
    } else {
        camera.view = glm::rotate(camera.view, (float) glm::radians(45.0), glm::vec3(1.f, 0.f, 0.f));
        camera.view = glm::translate(camera.view, glm::vec3(0.0f, -100.0f, -100.0f));
    }

    if (position.x <= -128.f || position.y <= -128.f || position.x >= 128.f || position.y >= 128.f) {
        die();
    }

    slither(position.x, position.y);

    if (window.keyDown(GLFW_KEY_SPACE)) {
        grow();
    }
}

void Snake::render(Shader shader) {
    
    glActiveTexture(GL_TEXTURE0);

    for (size_t i = 0; i < positions.size(); i++) {
        !i ? m_texture_head.bind() : m_texture_body.bind();
    
        float angle = 0.0f;
        if (i < positions.size() - 1) {
            glm::vec2 nextSegment = positions[i + 1];
            glm::vec2 thisSegment = positions[i];
    
            float dx = nextSegment.x - thisSegment.x;
            float dy = nextSegment.y - thisSegment.y;
    
            angle = atan2f(dy, dx);
        } else if (i > 0) {
            glm::vec2 thisSegment = positions[i];
            glm::vec2 prevSegment = positions[i - 1];
    
            float dx = thisSegment.x - prevSegment.x;
            float dy = thisSegment.y - prevSegment.y;
    
            angle = atan2f(dy, dx);
        }
    
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(positions[i].x, !i ? 1.9f : 2.f, positions[i].y));
        model = glm::rotate(model, angle, glm::vec3(0.f, 1.f, 0.f));
        model = glm::scale(model, glm::vec3(segmentSize*2));
    
        m_model.render(shader, model);
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

void Snake::die() {
    // TODO: die
}