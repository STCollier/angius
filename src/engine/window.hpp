#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_set>
#include <glm/glm.hpp>

class Window {
    public:
        std::string title;
        std::unordered_set<int> keyPressed;
        std::unordered_set<int> mousePressed;
        double mouseX, mouseY;

        int width;
        int height;

        Window(std::string title, int width, int height);
        ~Window();

        void update();
        float deltaTime();
        float time();
        bool keyDown(int key);
        bool mouseDown(int button);
        
        GLFWwindow* getWindow();
        virtual void sizeCallback();
        virtual void keyboardCallback(int key, int scancode, int action, int mode);
        virtual void mouseCallback(int button, int action, int mode);
    private:
        GLFWwindow* m_window;
        float lastFrame, dt;

        static void sizeCallback(GLFWwindow *window, int width, int height);
        static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void mouseCallback(GLFWwindow* window, int button, int action, int mode);
};