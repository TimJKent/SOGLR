#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Math/Vector.h"
#include <iostream>

namespace SOGLR
{
    class Window
    {
    public:
        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
        }

        static void ErrorCallback(int error, const char *description)
        {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        }

        Window(int width = 600, int height = 400, const char *title = "SOGLR")
        {
            glfwSetErrorCallback(ErrorCallback);
            if (!glfwInit())
            {
                std::cerr << "Issue with initlizing GLFW" << std::endl;
                return;
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            window_ = glfwCreateWindow(width, height, title, NULL, NULL);
            is_valid_ = (window_ != nullptr);
            if (is_valid_)
            {
                glfwSetKeyCallback(window_, KeyCallback);
                glfwMakeContextCurrent(window_);

                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                glfwSwapInterval(0);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_FRAMEBUFFER_SRGB);
                glEnable(GL_MULTISAMPLE);
                glEnable(GL_CULL_FACE);
                glfwWindowHint(GLFW_SAMPLES, 4);
                glCullFace(GL_BACK);
            }
        }

        ~Window()
        {
            Destroy();
        }

        void Destroy()
        {
            if (is_valid_)
            {
                glfwDestroyWindow(window_);
                is_valid_ = false;
            }
        }

        void BeginFrame()
        {
            Vector2Int window_size = GetSize();
            glViewport(0, 0, static_cast<GLsizei>(window_size.x), static_cast<GLsizei>(window_size.y));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.44f, 0.74f, 0.88f, 1.0f);
        }

        void EndFrame()
        {
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }

        Vector2Int GetSize() const
        {
            Vector2Int size;
            glfwGetWindowSize(window_, (int *)&size.x, (int *)&size.y);
            return size;
        }

        float GetAspectRatio() const
        {
            int width;
            int height;
            glfwGetWindowSize(window_, &width, &height);
            return static_cast<float>(width) / static_cast<float>(height);
        }

        bool ShouldClose() const
        {
            return glfwWindowShouldClose(window_);
        }

        bool IsKeyDown(int key) const
        {
            return glfwGetKey(window_, key) == GLFW_PRESS;
        }

        bool IsValid() const { return is_valid_; }

        void Resize(uint32_t width, uint32_t height)
        {
            glfwSetWindowSize(window_, width, height);
        }

        void SetTitle(const std::string &title)
        {
            glfwSetWindowTitle(window_, title.c_str());
        }

    private:
        GLFWwindow *window_;
        bool is_valid_ = false;
    };
}