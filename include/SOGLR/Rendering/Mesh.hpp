#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <iostream>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/VertexArrayObject.hpp"
#include "Rendering/Vertex.hpp"

namespace Rendering
{

    struct Texture
    {
        uint32_t id_;
    };

    class Mesh
    {
    public:
        Mesh()
        {
        }
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<Texture> &textures)
        {
            vertex_buffer_ = std::make_shared<VertexBuffer>(vertices);
            index_buffer_ = std::make_shared<IndexBuffer>(indices);
            vao_ = std::make_shared<VertexArrayObject>();
            vao_->AddVertexBuffer(vertex_buffer_);
            vao_->SetIndexBuffer(index_buffer_);
        }

        void Draw(Shader &shader)
        {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            // for (unsigned int i = 0; i < textures.size(); i++)
            //{
            //     glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            //     // retrieve texture number (the N in diffuse_textureN)
            //     string number;
            //     string name = textures[i].type;
            //     if (name == "texture_diffuse")
            //         number = std::to_string(diffuseNr++);
            //     else if (name == "texture_specular")
            //         number = std::to_string(specularNr++);
            //
            //    shader.setInt(("material." + name + number).c_str(), i);
            //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
            //}
            // glActiveTexture(GL_TEXTURE0);

            // draw mesh
            vao_->Bind();
            glDrawElements(GL_TRIANGLES, index_buffer_->Count(), GL_UNSIGNED_INT, 0);
            vao_->Unbind();
        }

        ~Mesh() = default;

    private:
        std::shared_ptr<VertexBuffer> vertex_buffer_;
        std::shared_ptr<IndexBuffer> index_buffer_;
        std::shared_ptr<VertexArrayObject> vao_;
    };
};