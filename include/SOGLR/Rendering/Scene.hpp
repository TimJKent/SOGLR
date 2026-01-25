#pragma once

#include <memory>
#include <iostream>
#include <vector>

#include "Rendering/Camera.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Lighting/DirectionalLight.hpp"
#include "Rendering/Advanced/Shadowmap.hpp"

namespace SOGLR
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        void SetSceneCamera(const std::shared_ptr<Camera> &camera) { scene_camera_ = camera; }

        void SetDirectionalLight(std::shared_ptr<Lighting::DirectionalLight> directional_light)
        {
            directional_light_ = directional_light;
        }

        void AddRenderObject(std::shared_ptr<RenderObject> render_object)
        {
            render_list_.push_back(render_object);
        }

        void AddRenderObject(std::vector<std::shared_ptr<RenderObject>> render_object)
        {
            for (auto& render_object : render_object)
            {
                render_list_.push_back(render_object);
            }
        }

        void DrawScene(std::shared_ptr<Advanced::Shadowmap> shadow_map = nullptr, glm::ivec2 viewport_size = glm::vec2(0.0f))
        {
            if (!scene_camera_)
            {
                std::cerr << "No Scene Camera!" << std::endl;
                return;
            }

            for (auto &obj : render_list_)
            {
                auto shader = obj->GetShader();
                shader->Bind();
                shader->SetInt("diffusedTexture", 0);
                shader->SetInt("normalTexture", 1);
                shader->SetUniformMat4f("uOrthoProjection", glm::ortho(0.0f, static_cast<float>(viewport_size.x), 0.0f, static_cast<float>(viewport_size.y), -1.0f, 100.0f));

                if (shadow_map)
                {
                    shader->SetInt("shadowMap", 5);
                    shader->SetUniformMat4f("lightSpaceMatrix", shadow_map->GetLightSpaceMatrix());
                }

                
                if (directional_light_)
                {
                    shader->SetUBOMatrices(scene_camera_->GetProjectionMatrix(), scene_camera_->GetViewMatrix(), scene_camera_->GetTransform().position, directional_light_->direction, directional_light_->color, directional_light_->intensity);
                }
                else
                {
                    shader->SetUBOMatrices(scene_camera_->GetProjectionMatrix(), scene_camera_->GetViewMatrix(), scene_camera_->GetTransform().position, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}, 0.0f);
                }

                obj->Draw();
            }
        }

        void DrawSceneFixedShader(std::shared_ptr<Shader> shader)
        {
            if (!scene_camera_)
            {
                std::cerr << "No Scene Camera!" << std::endl;
                return;
            }

            if (!directional_light_)
            {
                std::cerr << "No Directional Light!" << std::endl;
                return;
            }

            for (auto &obj : render_list_)
            {
                shader->Bind();
                shader->SetUBOMatrices(scene_camera_->GetProjectionMatrix(), scene_camera_->GetViewMatrix(), scene_camera_->GetTransform().position, directional_light_->direction, directional_light_->color, directional_light_->intensity);
                obj->Draw();
            }
        }

        std::shared_ptr<Camera> GetCamera()
        {
            return scene_camera_;
        }

        std::shared_ptr<Lighting::DirectionalLight> GetDirectionalLight()
        {
            return directional_light_;
        }

    private:
        std::shared_ptr<Camera>
            scene_camera_;
        std::vector<std::shared_ptr<RenderObject>> render_list_;
        std::shared_ptr<Lighting::DirectionalLight> directional_light_;
    };
}