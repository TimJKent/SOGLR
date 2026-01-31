#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <array>

namespace SOGLR
{
    class Window
    {
    public:
        enum class KeyAction
        {
            Unknown = -1,
            Release = GLFW_RELEASE,
            Press = GLFW_PRESS,
            Repeat = GLFW_REPEAT
        };

        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                win->key_states_[key] = static_cast<KeyAction>(action);
                if (win)
                {
                    if (win->key_callbacks_.count(key) == 0)
                        return;

                    for (auto &callback : win->key_callbacks_[key])
                        callback(static_cast<KeyAction>(action));
                }
            }
        }

        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
        {
            Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            if (win)
            {
                for (auto &callback : win->window_resize_callbacks_)
                {
                    callback(glm::ivec2(width, height));
                }
            }
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
                glfwSetWindowUserPointer(window_, this);
                glfwSetKeyCallback(window_, KeyCallback);
                glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
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
            glm::ivec2 window_size = GetSize();
            glViewport(0, 0, static_cast<GLsizei>(window_size.x), static_cast<GLsizei>(window_size.y));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
        }

        void EndFrame()
        {
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }

        glm::ivec2 GetSize() const
        {
            glm::ivec2 size;
            glfwGetWindowSize(window_, (int *)&size.x, (int *)&size.y);
            return size;
        }

        glm::ivec2 GetMousePosition() const
        {
            glm::ivec2 size;
            glfwGetWindowSize(window_, (int *)&size.x, (int *)&size.y);
            double xpos, ypos;
            glfwGetCursorPos(window_, &xpos, &ypos);
            return glm::ivec2(static_cast<int>(xpos), size.y - static_cast<int>(ypos));
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

        void RequestWindowClose()
        {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        bool IsKeyDown(int key) const
        {
            return key_states_[key] == KeyAction::Press;
        }

        bool IsKeyHeld(int key) const
        {
            return key_states_[key] == KeyAction::Repeat || key_states_[key] == KeyAction::Press;
        }

        bool IsKeyUp(int key) const
        {
            return key_states_[key] == KeyAction::Release;
        }

        bool IsMouseButtonDown(int button) const
        {
            return glfwGetMouseButton(window_, button) == GLFW_PRESS;
        }

        bool IsMouseButtonUp(int button) const
        {
            return glfwGetMouseButton(window_, button) == GLFW_RELEASE;
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

        void SetSize(uint32_t width, uint32_t height)
        {
            glfwSetWindowSize(window_, width, height);
        }

        void SetClearColor(const glm::vec4 &color)
        {
            clear_color_ = color;
        }

        void AddKeyCallback(int key, std::function<void(Window::KeyAction)> callback)
        {
            key_callbacks_[key].push_back(callback);
        }

        void AddWindowResizeCallback(std::function<void(glm::ivec2)> callback)
        {
            window_resize_callbacks_.push_back(callback);
        }

    private:
        GLFWwindow *window_;
        bool is_valid_ = false;
        glm::vec4 clear_color_ = {0.44f, 0.74f, 0.88f, 1.0f};
        std::unordered_map<int, std::vector<std::function<void(KeyAction)>>> key_callbacks_;
        std::vector<std::function<void(glm::ivec2)>> window_resize_callbacks_;
        std::array<KeyAction, GLFW_KEY_LAST> key_states_{KeyAction::Unknown};
    };
}