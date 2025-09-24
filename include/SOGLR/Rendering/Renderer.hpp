#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <memory>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/VertexArrayObject.hpp"
#include "Rendering/RenderObject.hpp"

namespace Rendering
{
    class Renderer
    {
    public:
        static void ErrorCallback(int error, const char *description)
        {
            std::cout << "Error: " << description << std::endl;
        }

        bool Begin()
        {
            glfwSetErrorCallback(ErrorCallback);

            if (!glfwInit())
            {
                return false;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            return true;
        }

        void DrawIndexed(const VertexArrayObject &vao)
        {
            vao.Bind();
            glDrawElements(GL_TRIANGLES, vao.IndexCount(), GL_UNSIGNED_INT, nullptr);
        }

        void DrawRenderObject(std::shared_ptr<RenderObject> obj)
        {
            obj->Bind();
            if (scene_camera_)
            {
                obj->GetShader()->SetUniformMat4f("uProjection", scene_camera_->GetProjectionMatrix());
                obj->GetShader()->SetUniformMat4f("uView", scene_camera_->GetViewMatrix());
            }
            glDrawElements(GL_TRIANGLES, obj->IndexCount(), GL_UNSIGNED_INT, nullptr);
        }

        void DrawRenderList()
        {
            for (auto &obj : render_list_)
            {
                DrawRenderObject(obj);
            }
        }

        void PushRenderObject(const std::shared_ptr<RenderObject> &obj)
        {
            render_list_.push_back(obj);
        }

        std::shared_ptr<RenderObject> CreateRenderObject(const std::shared_ptr<IndexBuffer> &ibo, const std::shared_ptr<VertexBuffer> &vbo, std::shared_ptr<Shader> shader)
        {
            std::shared_ptr<RenderObject> obj = std::make_shared<RenderObject>();
            obj->SetModelData(ibo, vbo);
            obj->SetShader(shader);
            render_list_.push_back(obj);
            return obj;
        }

        void BeginFrame()
        {
            frame_start_time_ = std::chrono::high_resolution_clock::now();
        }

        void EndFrame()
        {
            auto frame_end_time = std::chrono::high_resolution_clock::now();
            delta_time_ = frame_end_time - frame_start_time_;
        }

        void End()
        {
            glfwTerminate();
        }

        void SetFillMode(uint32_t mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, mode);
        }

        float GetDeltaTime() const { return delta_time_.count(); }

        bool IsValid() const { return is_valid_; }

        void SetSceneCamera(std::shared_ptr<Camera> camera)
        {
            scene_camera_ = camera;
        }

    private:
        std::vector<std::shared_ptr<RenderObject>> render_list_;
        std::shared_ptr<Camera> scene_camera_;
        std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_time_;
        std::chrono::duration<float> delta_time_;
        bool is_valid_ = false;
    };
}