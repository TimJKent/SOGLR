#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include "Rendering/Texture.hpp"

namespace SOGLR
{
    namespace Text
    {
        struct Character
        {
            std::shared_ptr<Texture> texture;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            uint32_t Advance;
        };
    }
}