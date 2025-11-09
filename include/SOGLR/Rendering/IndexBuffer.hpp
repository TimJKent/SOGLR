#pragma once
#include <cstdint>
#include <vector>

#include "glad/glad.h"

namespace SOGLR
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const std::vector<GLuint> &data)
            : count_(data.size())
        {
            glGenBuffers(1, &renderer_id_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
        }

        ~IndexBuffer()
        {
            glDeleteBuffers(1, &renderer_id_);
        }

        void Bind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
        }

        void Unbind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        uint32_t Count() const
        {
            return count_;
        }

    private:
        uint32_t renderer_id_ = 0;
        uint32_t count_ = 0;
    };
}