#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/camera.hpp"
#include "engine/lighting.hpp"
#include "engine/model.hpp"
#include "engine/util.hpp"

#include "anguis/ground.hpp"
#include "anguis/snake.hpp"
#include "anguis/food.hpp"

int main() {
    Window window("Window", 1600, 900);
    Shader mainShader("res/shaders/main.vert", "res/shaders/main.frag");
    Shader depthShader("res/shaders/depth.vert", "res/shaders/depth.frag");
    Shader blurShader("res/shaders/blur.vert", "res/shaders/blur.frag");
    Shader bloomShader("res/shaders/bloom.vert", "res/shaders/bloom.frag");
    Camera camera(window);
    Lighting lighting(window,
        depthShader,
        blurShader,
        bloomShader
    );

    Ground ground{};
    Snake snake{};
    Food food{};

    mainShader.use();
    camera.setMatrices(mainShader);
    mainShader.setInt("_texture", 0);
    mainShader.setInt("shadowMap", 1);

    bool bloom = true;
    bool bloomKeyPressed = false;

    while (!glfwWindowShouldClose(window.getWindow())) {
        float dt = window.deltaTime();
        float time = window.time();

        window.update();
        camera.update(window);
        
        // First pass: render scene from perspective of light to depth buffer
        lighting.shadowPass(camera);

        glCullFace(GL_FRONT);
        ground.render(depthShader);
        snake.render(depthShader);
        food.render(depthShader, snake, time);
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_POLYGON_OFFSET_FILL);

        glm::ivec2 framebufferSize;
        glfwGetFramebufferSize(window.getWindow(), &framebufferSize.x, &framebufferSize.y);
        glViewport(0, 0, framebufferSize.x, framebufferSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Second pass: render scene from normal camera's perspective to HDR framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, lighting.getHDRFBO());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mainShader.use();
        camera.setMatrices(mainShader);
        mainShader.setVec3("viewPos", camera.position);
        mainShader.setVec3("lightPos", lighting.lightPos);
        mainShader.setMat4("lightSpaceMatrix", lighting.lightSpaceMatrix);
        mainShader.setVec2("resolution", glm::vec2(framebufferSize.x, framebufferSize.y));
    
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lighting.getDepthBuffer());

        ground.render(mainShader);
        snake.render(mainShader);
        food.render(mainShader, snake, time);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Blur bright fragments using blur shader
        lighting.blurBloom();

        // Render out tonemapped HDR color
        lighting.renderColor();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_1) == GLFW_PRESS && !bloomKeyPressed) {
            bloom = !bloom;
            bloomKeyPressed = true;
        }
        if (glfwGetKey(window.getWindow(), GLFW_KEY_1) == GLFW_RELEASE) {
            bloomKeyPressed = false;
        }

        bloomShader.setInt("useBloom", bloom);

        snake.update(window, dt, camera);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    return 0;
}