#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Math/Vector.h"
#include <iostream>

namespace Rendering
{
    class Window
    {
    public:
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            std::cout << "Key: " << key << " Action: " << action << std::endl;
        }

        Window::Window(int width, int height, const char *title)
        {
            window_ = glfwCreateWindow(width, height, title, NULL, NULL);
            is_valid_ = (window_ != nullptr);
            if (is_valid_)
            {
                glfwSetKeyCallback(window_, key_callback);
                glfwMakeContextCurrent(window_);

                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                glfwSwapInterval(0);
                glEnable(GL_DEPTH_TEST);
            }
        }

        Window::~Window()
        {
            glfwDestroyWindow(window_);
        }

        void Window::BeginFrame()
        {
            Vector2Int window_size = GetSize();
            glViewport(0, 0, static_cast<GLsizei>(window_size.x), static_cast<GLsizei>(window_size.y));

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Window::EndFrame()
        {
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }

        Vector2Int Window::GetSize() const
        {
            Vector2Int size;
            glfwGetWindowSize(window_, (int *)&size.x, (int *)&size.y);
            return size;
        }

        float Window::GetAspectRatio() const
        {
            int width;
            int height;
            glfwGetWindowSize(window_, &width, &height);
            return static_cast<float>(width) / static_cast<float>(height);
        }

        bool Window::ShouldClose() const
        {
            return glfwWindowShouldClose(window_);
        }

        bool IsKeyDown(int key) const
        {
            return glfwGetKey(window_, key) == GLFW_PRESS;
        }

        bool IsValid() const { return is_valid_; }

    private:
        GLFWwindow *window_;
        bool is_valid_ = false;
    };
}