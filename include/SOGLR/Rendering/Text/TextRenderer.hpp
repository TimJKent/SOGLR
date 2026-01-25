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