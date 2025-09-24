#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Rendering/Transform.hpp"

namespace Rendering
{
    class Camera
    {
    public:
        Camera() = default;
        ~Camera() = default;

        Rendering::Transform &GetTransform() { return transform_; }
        glm::mat4 GetProjectionMatrix() { return glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 100.0f); }
        glm::mat4 GetViewMatrix()
        {
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::rotate(view, glm::radians(-transform_.rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
            view = glm::rotate(view, glm::radians(-transform_.rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
            view = glm::rotate(view, glm::radians(-transform_.rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
            view = glm::translate(view, -transform_.position_);
            return view;
        }

    private:
        Rendering::Transform transform_;
        float aspectRatio = 1.0f;
    };
}