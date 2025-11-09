#pragma once

#include <memory>

#include <SOGLR/Rendering/Camera.hpp>
#include <SOGLR/Rendering/Window.hpp>

namespace SOGLR
{
    class PerspectiveCameraController
    {
    public:
        PerspectiveCameraController()
            : camera_(std::make_shared<Camera>())
        {
        }
        ~PerspectiveCameraController() = default;
        std::shared_ptr<Camera> GetCamera() const { return camera_; }

        void SetPosition(const glm::vec3 &position)
        {
            camera_->GetTransform().position = position;
        }

        glm::vec3 GetPosition() const
        {
            return camera_->GetTransform().position;
        }

        void Update(const Window &window, const float &delta_time)
        {
            camera_->SetAspectRatio(window.GetAspectRatio());
            UpdateTransform(window, delta_time);
        }

    private:
        void UpdateTransform(const Window &window, const float &delta_time)
        {
            glm::vec3 position = camera_->GetTransform().position;
            glm::vec3 rotation = camera_->GetTransform().rotation;

            if (window.IsKeyDown(GLFW_KEY_W))
            {
                position += camera_->GetTransform().Forward() * delta_time * 10.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_S))
            {
                position -= camera_->GetTransform().Forward() * delta_time * 10.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_A))
            {
                position -= camera_->GetTransform().Right() * delta_time * 10.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_D))
            {
                position += camera_->GetTransform().Right() * delta_time * 10.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_Q))
            {
                rotation.y += delta_time * 100.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_E))
            {
                rotation.y -= delta_time * 100.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_R))
            {
                position += camera_->GetTransform().Up() * delta_time * 10.0f;
            }
            if (window.IsKeyDown(GLFW_KEY_F))
            {
                position -= camera_->GetTransform().Up() * delta_time * 10.0f;
            }

            camera_->GetTransform().position = position;
            camera_->GetTransform().rotation = rotation;
        }

        std::shared_ptr<Camera> camera_;
    };
}