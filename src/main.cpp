#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/camera.hpp"
#include "engine/lighting.hpp"

#include "anguis/ground.hpp"
#include "anguis/snake.hpp"


int main() {
    Window window("Window", 1600, 900);
    Shader mainShader("res/shaders/main.vert", "res/shaders/main.frag");
    Shader depthShader("res/shaders/depth.vert", "res/shaders/depth.frag");
    Camera camera(window);
    Lighting lighting{};

    Ground ground{};
    Snake snake{};

    mainShader.use();
    camera.setMatrices(mainShader);
    mainShader.setInt("_texture", 0);
    mainShader.setInt("shadowMap", 1);

    while (!glfwWindowShouldClose(window.getWindow())) {
        window.update();
        camera.update(window);

        lighting.shadowPass(depthShader, camera);

        glCullFace(GL_FRONT);
        ground.render(depthShader);
        snake.render(depthShader);
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glm::ivec2 framebufferSize;
        glfwGetFramebufferSize(window.getWindow(), &framebufferSize.x, &framebufferSize.y);
        glViewport(0, 0, framebufferSize.x, framebufferSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mainShader.use();
        camera.setMatrices(mainShader);
        mainShader.setVec3("viewPos", glm::vec3(0.0f, 100.0f, 100.0f));
        mainShader.setVec3("lightPos", lighting.lightPos);
        mainShader.setMat4("lightSpaceMatrix", lighting.lightSpaceMatrix);
    
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lighting.getDepthMap());

        ground.render(mainShader);
        snake.render(mainShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        snake.update(window, window.deltaTime(), camera);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    return 0;
}