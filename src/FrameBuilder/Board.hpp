#pragma once
#include <cstdint>
#include "Math/Vector.h" 

namespace FrameBuilder
{
    class Board
    {
    public:
        Board() = default;

        ~Board() = default;

        const Vector3& Size() const { return size_; }

        std::vector<float> GetVertices() { return 
        {
           // Front face (z = +0.5, normal (0,0,1))
            -size_.x, -size_.y, size_.z, 0, 0, 1,
             size_.x, -size_.y, size_.z, 0, 0, 1,
             size_.x,  size_.y, size_.z, 0, 0, 1,
            -size_.x,  size_.y, size_.z, 0, 0, 1,

            // Back face (z = -0.5, normal (0,0,-1))
             size_.x, -size_.y, -size_.z,  0, 0, -1,
            -size_.x, -size_.y, -size_.z,  0, 0, -1,
            -size_.x,  size_.y, -size_.z,  0, 0, -1,
             size_.x,  size_.y, -size_.z,  0, 0, -1,

            // Left face (x = -0.5, normal (-1,0,0))
            -size_.x, -size_.y, -size_.z, -1, 0, 0,
            -size_.x, -size_.y,  size_.z, -1, 0, 0,
            -size_.x,  size_.y,  size_.z, -1, 0, 0,
            -size_.x,  size_.y, -size_.z, -1, 0, 0,

            // Right face (x = +0.5, normal (1,0,0))
             size_.x, -size_.y,  size_.z, 1, 0, 0,
             size_.x, -size_.y, -size_.z, 1, 0, 0,
             size_.x,  size_.y, -size_.z, 1, 0, 0,
             size_.x,  size_.y,  size_.z, 1, 0, 0,

            // Top face (y = +0.5, normal (0,1,0))
            -size_.x, size_.y,  size_.z, 0, 1, 0,
             size_.x, size_.y,  size_.z, 0, 1, 0,
             size_.x, size_.y, -size_.z, 0, 1, 0,
            -size_.x, size_.y, -size_.z, 0, 1, 0,

            // Bottom face (y = -0.5, normal (0,-1,0))
            -size_.x,-size_.y, -size_.z, 0, -1, 0,
             size_.x,-size_.y, -size_.z, 0, -1, 0,
             size_.x,-size_.y,  size_.z, 0, -1, 0,
            -size_.x,-size_.y,  size_.z, 0, -1, 0,
        }; 
        }

        std::vector<uint32_t> GetIndex() { return 
            {
                0,1,2, 2,3,0,
                4,5,6, 6,7,4,
                8,9,10, 10,11,8,
                12,13,14, 14,15,12,
                16,17,18, 18,19,16,
                20,21,22, 22,23,20
            }; 
        }

    private:
       Vector3 size_ = {0.5f, 0.5f, 0.5f};
    };
} // namespace FrameBuilder