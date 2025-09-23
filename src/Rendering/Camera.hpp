#pragma once

#include <glm/glm.hpp>

#include "Rendering/Transform.hpp"

namespace Rendering
{
    class Camera
    {
    public:
        Camera() = default;
        ~Camera() = default;

        Rendering::Transform &GetTransform() { return transform_; }

    private:
        Rendering::Transform transform_;
    };
}