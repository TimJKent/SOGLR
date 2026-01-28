#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <memory>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArrayObject.hpp"
#include "Framebuffer.hpp"
#include "Advanced/Shadowmap.hpp"
#include "RenderObject.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Primitives.hpp"

namespace SOGLR
{
    class Renderer
    {
    public:
        Renderer()
        {
            window_ = std::make_shared<Window>();
            fullscreen_quad_vb_ = std::make_unique<VertexBuffer>(Primitives::Quad::vertices);
            fullscreen_quad_ib_ = std::make_unique<IndexBuffer>(Primitives::Quad::indicies);
            fullscreen_quad_vao_ = std::make_unique<VertexArrayObject>();
            fullscreen_quad_vao_->AddVertexBuffer(*fullscreen_quad_vb_.get());
            fullscreen_quad_vao_->SetIndexBuffer(*fullscreen_quad_ib_.get());
        }

        ~Renderer()
        {
            window_->Destroy();
            glfwTerminate();
        }

        static void ErrorCallback(int error, const char *description)
        {
            std::cout << "Error: " << description << std::endl;
        }

        void DrawIndexed(const VertexArrayObject &vao)
        {
            vao.Bind();
            glDrawElements(GL_TRIANGLES, vao.IndexCount(), GL_UNSIGNED_INT, nullptr);
        }

        void DrawRenderObject(std::shared_ptr<RenderObject> obj)
        {
            obj->Draw();
        }

        void DrawRenderList()
        {
            for (auto &obj : render_list_)
            {
                DrawRenderObject(obj);
            }
        }

        void DrawPolygon(std::vector<glm::vec3> positions)
        {
            glBegin(GL_TRIANGLES);
        }

        void PushRenderObject(const std::shared_ptr<RenderObject> &obj)
        {
            render_list_.push_back(obj);
        }

        void BeginFrame()
        {
            frame_start_time_ = std::chrono::high_resolution_clock::now();
            window_->BeginFrame();
        }

        void EndFrame()
        {
            window_->EndFrame();
            auto frame_end_time = std::chrono::high_resolution_clock::now();
            delta_time_ = frame_end_time - frame_start_time_;
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

        bool IsRunning()
        {
            return !window_->ShouldClose();
        }

        void Stop()
        {
            window_->RequestWindowClose();
        }

        std::shared_ptr<Window> &GetWindow()
        {
            return window_;
        }

        void DrawSceneToFramebuffer(Scene &scene, std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Advanced::Shadowmap> shadow_map, std::shared_ptr<Shader> depth_shader)
        {
            if (shadow_map && depth_shader)
            {
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                depth_shader->Bind();
                depth_shader->SetUniformMat4f("lightSpaceMatrix", shadow_map->GetLightSpaceMatrix());
                shadow_map->Bind();
                scene.DrawSceneFixedShader(depth_shader);
                depth_shader->Unbind();
                shadow_map->Unbind();
                glCullFace(GL_BACK);
                glm::ivec2 size = window_->GetSize();
                framebuffer->Invalidate(size.x, size.y);
            }
            framebuffer->Bind();
            scene.DrawScene(shadow_map);
            framebuffer->Unbind();
        }

        void DrawFramebufferToScreen(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Shader> framebuffer_shader)
        {
            framebuffer_shader->Bind();
            framebuffer_shader->SetInt("screenTexture", 0);
            framebuffer->BindTexture();
            DrawIndexed(*fullscreen_quad_vao_.get());
            framebuffer_shader->Unbind();
        }

        void DrawShadowmapToScreen(std::shared_ptr<Shader> framebuffer_shader, std::shared_ptr<Advanced::Shadowmap> shadow_map)
        {
            framebuffer_shader->Bind();
            framebuffer_shader->SetInt("screenTexture", 5);
            shadow_map->BindTexture();
            DrawIndexed(*fullscreen_quad_vao_.get());
            framebuffer_shader->Unbind();
        }

    private:
        std::shared_ptr<Window> window_;
        std::vector<std::shared_ptr<RenderObject>> render_list_;
        std::shared_ptr<Camera> scene_camera_;
        std::chrono::time_point<std::chrono::high_resolution_clock> frame_start_time_;
        std::chrono::duration<float> delta_time_;
        bool is_valid_ = false;
        std::unique_ptr<VertexBuffer> fullscreen_quad_vb_;
        std::unique_ptr<IndexBuffer> fullscreen_quad_ib_;
        std::unique_ptr<VertexArrayObject> fullscreen_quad_vao_;
    };
}