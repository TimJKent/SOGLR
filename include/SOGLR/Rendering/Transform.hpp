#pragma once

#include <glm/glm.hpp>

namespace Rendering
{
    class Transform
    {
    public:
        Transform() = default;
        ~Transform() = default;

        glm::vec3 Forward() const
        {
            glm::vec3 forward;
            forward.x = -sin(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
            forward.y = sin(glm::radians(rotation_.x));
            forward.z = -cos(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
            return forward;
        }

        glm::vec3 Right() const
        {
            glm::vec3 right;
            right.x = cos(glm::radians(rotation_.y));
            right.y = 0.0f;
            right.z = -sin(glm::radians(rotation_.y));
            return right;
        }

        glm::vec3 Up() const
        {
            return glm::cross(Right(), Forward());
        }

    public:
        glm::vec3 position_ = glm::vec3(0.0f);
        glm::vec3 rotation_ = glm::vec3(0.0f);
        glm::vec3 scale_ = glm::vec3(1.0f);
    };
}