#pragma once

#include "glm/glm.hpp"

namespace SOGLR
{
    namespace Lighting
    {
        struct DirectionalLight
        {
            glm::vec3 direction;
            glm::vec3 color;
            float intensity;
        };
    }
}