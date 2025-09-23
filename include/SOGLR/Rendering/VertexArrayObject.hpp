#pragma once

#include <cstdint>
#include <glad/glad.h>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/IndexBuffer.hpp"

namespace Rendering
{
    class VertexArrayObject
    {
    public:
        VertexArrayObject()
        {
            glGenVertexArrays(1, &renderer_id_);
        }

        ~VertexArrayObject()
        {
            glDeleteVertexArrays(1, &renderer_id_);
        }

        void AddVertexBuffer(const VertexBuffer &vb)
        {
            glBindVertexArray(renderer_id_);
            vb.Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        }

        void SetIndexBuffer(const IndexBuffer &index_buffer)
        {
            index_count_ = index_buffer.Count();
            glBindVertexArray(renderer_id_);
            index_buffer.Bind();
        }

        void Bind() const
        {
            glBindVertexArray(renderer_id_);
        }

        uint32_t IndexCount() const
        {
            return index_count_;
        }

        void Unbind() const
        {
            glBindVertexArray(0);
        }

    private:
        uint32_t renderer_id_ = 0;
        uint32_t index_count_ = 0;
    };

} // namespace Rendering
