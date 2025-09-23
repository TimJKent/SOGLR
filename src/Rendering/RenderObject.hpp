#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Rendering/Shader.hpp"
#include "Rendering/VertexArrayObject.hpp"
#include "Rendering/IndexBuffer.hpp"
#include "Rendering/VertexBuffer.hpp"

#include <memory>

namespace Rendering
{
    class RenderObject
    {
    public:
        RenderObject() = default;

        void SetShader(const std::shared_ptr<Shader> &shader) { shader_ = shader; }

        void SetModelData(const IndexBuffer &ibo, const VertexBuffer &vbo)
        {
            index_count_ = ibo.Count();
            vao_.AddVertexBuffer(vbo);
            vao_.SetIndexBuffer(ibo);
        }

        void SetPosition(const glm::vec3 &position) { position_ = position; }
        void SetRotation(const glm::vec3 &rotation) { rotation_ = rotation; }
        void SetScale(const glm::vec3 &scale) { scale_ = scale; }

        glm::vec3 GetPosition() const { return position_; }

        void Bind() const
        {
            if (shader_)
            {
                shader_->Bind();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, scale_);
                model = glm::rotate(model, glm::radians(rotation_.x), glm::vec3(1, 0, 0));
                model = glm::rotate(model, glm::radians(rotation_.y), glm::vec3(0, 1, 0));
                model = glm::rotate(model, glm::radians(rotation_.z), glm::vec3(0, 0, 1));
                model = glm::translate(model, position_);
                shader_->SetUniformMat4f("uModel", model);
            }
            vao_.Bind();
        }

        uint32_t IndexCount() const { return index_count_; }

    private:
        std::shared_ptr<Shader> shader_;
        Rendering::VertexArrayObject vao_;
        uint32_t index_count_ = 0;
        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
    };
}