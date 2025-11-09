#pragma once

#include <cstdint>
#include <glad/glad.h>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/IndexBuffer.hpp"

namespace SOGLR
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
            vao_size = sizeof(glm::vec3) * 4 + sizeof(glm::vec2);
            glBindVertexArray(renderer_id_);
            vb.Bind();

            AddAttribute(3);
            AddAttribute(3);

            AddAttribute(2);
            AddAttribute(3);
            AddAttribute(3);
        }

        void SetIndexBuffer(const IndexBuffer &index_buffer)
        {
            glBindVertexArray(renderer_id_);
            index_buffer.Bind();
            index_count_ = index_buffer.Count();
            glBindVertexArray(0);
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
        void AddAttribute(uint32_t attribute_size)
        {
            glEnableVertexAttribArray(attribute_count);
            glVertexAttribPointer(attribute_count, attribute_size, GL_FLOAT, GL_FALSE, vao_size, (void *)attribute_size_offset);
            attribute_size_offset += attribute_size * sizeof(float);
            attribute_count++;
        }
        uint32_t renderer_id_ = 0;
        uint32_t index_count_ = 0;
        uint32_t attribute_count = 0;
        uint32_t attribute_size_offset = 0;
        uint32_t vao_size = 0;
    };

} // namespace SOGLR
