#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Rendering/Shader.hpp"
#include "Rendering/Model.hpp"

#include <memory>
#include <iostream>

namespace SOGLR
{
    class RenderObject
    {
    public:
        RenderObject() = default;

        void SetShader(const std::shared_ptr<Shader> &shader) { shader_ = shader; }

        void SetModel(const std::shared_ptr<Model> &model)
        {
            model_ = model;
        }

        void Bind() const
        {
            if (shader_)
            {
                shader_->Bind();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, transform_.position);
                model = glm::rotate(model, glm::radians(transform_.rotation.x), glm::vec3(1, 0, 0));
                model = glm::rotate(model, glm::radians(transform_.rotation.y), glm::vec3(0, 1, 0));
                model = glm::rotate(model, glm::radians(transform_.rotation.z), glm::vec3(0, 0, 1));
                model = glm::scale(model, transform_.scale);
                shader_->SetUniformMat4f("uModel", model);
            }
            else
            {
                std::cout << "Shader not bound for RenderObject!" << std::endl;
            }
        }

        void Draw()
        {
            Bind();
            model_->Draw(shader_);
        }

        std::shared_ptr<Shader> GetShader() { return shader_; }

        Transform &GetTransform() { return transform_; }

    private:
        std::shared_ptr<Shader> shader_;
        std::shared_ptr<Model> model_;
        Transform transform_;
    };
}