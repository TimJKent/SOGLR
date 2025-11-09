#pragma once
#include <cstdint>
#include <vector>
#include "Math/Vector.h"
#include <glad/glad.h>

#include "Rendering/Vertex.hpp"

namespace SOGLR
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const std::vector<Vertex> &verts)
            : count_(static_cast<uint32_t>(verts.size()))
        {
            glGenBuffers(1, &render_id_);
            glBindBuffer(GL_ARRAY_BUFFER, render_id_);
            glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
        }

        ~VertexBuffer()
        {
            glDeleteBuffers(1, &render_id_);
        }

        void Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, render_id_);
        }

        void Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        uint32_t Count() const { return count_; }

    private:
        uint32_t render_id_;
        uint32_t count_;
    };
}