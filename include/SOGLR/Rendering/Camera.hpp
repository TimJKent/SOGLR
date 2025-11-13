#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.hpp"

namespace SOGLR
{
    class Camera
    {
    public:
        Camera() = default;
        ~Camera() = default;

        Transform &GetTransform() { return transform_; }
        glm::mat4 GetProjectionMatrix() { return glm::perspective(glm::radians(90.0f), aspect_ratio_, 0.1f, 1000.0f); }
        glm::mat4 GetViewMatrix()
        {
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::rotate(view, glm::radians(-transform_.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
            view = glm::rotate(view, glm::radians(-transform_.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
            view = glm::rotate(view, glm::radians(-transform_.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
            view = glm::translate(view, -transform_.position);
            return view;
        }

        void SetAspectRatio(float aspect_ratio)
        {
            aspect_ratio_ = aspect_ratio;
        }

    private:
        Transform transform_;
        float aspect_ratio_ = 1.0f;
    };
}