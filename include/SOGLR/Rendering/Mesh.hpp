#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <iostream>

#include "Rendering/VertexBuffer.hpp"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/VertexArrayObject.hpp"

namespace Rendering
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture
    {
        uint32_t id_;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices_;
        std::vector<uint32_t> indices_;
        std::vector<Texture> textures_;

        Mesh()
        {
        }
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
            : vertices_(vertices), indices_(indices), textures_(textures)
        {

            std::vector<float> verts;

            for (auto &v : vertices_)
            {
                verts.push_back(v.position.x);
                verts.push_back(v.position.y);
                verts.push_back(v.position.z);
                verts.push_back(v.normal.x + 0.1f);
                verts.push_back(v.normal.y);
                verts.push_back(v.normal.z);
                verts.push_back(v.texCoords.x);
                verts.push_back(v.texCoords.y);
            }
            vertex_buffer_ = std::make_shared<VertexBuffer>(verts);
            index_buffer_ = std::make_shared<IndexBuffer>(indices_);
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