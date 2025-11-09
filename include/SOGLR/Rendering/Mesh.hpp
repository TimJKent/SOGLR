#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <iostream>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/VertexArrayObject.hpp"
#include "Rendering/Vertex.hpp"
#include "Rendering/Texture.hpp"

namespace SOGLR
{
    class Mesh
    {
    public:
        Mesh() = delete;

        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<std::shared_ptr<Texture>> &textures)
            : textures_(textures), vertex_buffer_(vertices), index_buffer_(indices)
        {
            vao_ = std::make_shared<VertexArrayObject>();
            vao_->AddVertexBuffer(vertex_buffer_);
            vao_->SetIndexBuffer(index_buffer_);
        }

        void Draw(Shader &shader)
        {
            // draw mesh
            vao_->Bind();
            shader.Bind();
            shader.SetInt("diffusedTexture", 0);
            shader.SetInt("normalTexture", 1);
            for (auto &tex : textures_)
            {
                tex->Bind();
            }
            glDrawElements(GL_TRIANGLES, index_buffer_.Count(), GL_UNSIGNED_INT, 0);
            vao_->Unbind();
        }

        ~Mesh() = default;

    private:
        VertexBuffer vertex_buffer_;
        IndexBuffer index_buffer_;
        std::vector<std::shared_ptr<Texture>> textures_;
        std::shared_ptr<VertexArrayObject> vao_;
    };
};