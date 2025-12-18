#pragma once
#include <string>
#include <memory>
#include "Rendering/Shader.hpp"
#include "Rendering/Text/Font.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/RenderObject.hpp"
#include "Rendering/Primitives.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/Scene.hpp"

namespace SOGLR::Text
{
    class TextRenderer
    {
        public:
            TextRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<Font> font, Scene& scene) 
            : shader_(shader)
            , font_(font)
            , text_obj_(std::make_shared<SOGLR::RenderObject>())
            {
                
                
                // Add to scene so it gets rendered with proper matrix setup
                scene.AddRenderObject(text_obj_);
            }

            void SetText(const std::string& new_text)
            {
                text_ = new_text;
                float x = 0.0f;
                float y = 0.0f;
                float scale = 0.002f;
                auto quad_model = std::make_shared<SOGLR::Model>();

                for(char c : text_)
                {
                    const Character& ch = font_->GetCharacter(c);
                
                    // Calculate character quad position and size
                    float xpos = x + ch.Bearing.x * scale;
                    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
                    float w = ch.Size.x * scale;
                    float h = ch.Size.y * scale;
                    
                    std::vector<Vertex> vertices = {
                        { xpos,     ypos + h, 0.0f, 0.0f,0.0f,1.0f, 0.0f, 1.0f,  1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f }, // top-left
                        { xpos + w, ypos + h, 0.0f, 0.0f,0.0f,1.0f, 1.0f, 1.0f,  1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f }, // top-right
                        { xpos + w, ypos,     0.0f, 0.0f,0.0f,1.0f, 1.0f, 0.0f,  1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f }, // bottom-right         
                        { xpos,     ypos,     0.0f, 0.0f,0.0f,1.0f, 0.0f, 0.0f,  1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f }, // bottom-left            
                    };
                    SOGLR::Mesh quad_mesh
                    {
                        vertices,
                        SOGLR::Primitives::Quad::indicies,
                        {ch.texture}
                    };
                    quad_model->AddMesh(quad_mesh);
                    x += (ch.Advance >> 6) * scale; 
                }
                
                text_obj_->SetModel(quad_model);
                text_obj_->SetShader(shader_);
            }

            std::shared_ptr<SOGLR::RenderObject> GetTextObject()
            {
                return text_obj_;
            }

        private:
            std::shared_ptr<Font> font_;
            std::shared_ptr<Shader> shader_;
            std::string text_;
            std::shared_ptr<SOGLR::RenderObject> text_obj_;
    };
}