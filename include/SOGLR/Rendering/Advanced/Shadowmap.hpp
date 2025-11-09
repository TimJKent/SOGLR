#pragma once

#include <cstdint>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "SOGLR/Rendering/Framebuffer.hpp"
#include "SOGLR/Rendering/Texture.hpp"
#include "SOGLR/Rendering/Lighting/DirectionalLight.hpp"

namespace SOGLR
{

    namespace Advanced
    {
        class Shadowmap
        {
        public:
            Shadowmap(uint32_t width, uint32_t height)
                : width_(width), height_(height)
            {
                glGenFramebuffers(1, &fbo_id_);

                glGenTextures(1, &depth_map_texture_);
                glBindTexture(GL_TEXTURE_2D, depth_map_texture_);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                             width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
                const float border[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

                glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture_, 0);
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
                // check completeness
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if (status != GL_FRAMEBUFFER_COMPLETE)
                {
                    std::cout << "Shadowmap FBO incomplete: 0x" << std::hex << status << std::dec << std::endl;
                }

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            ~Shadowmap() {}

            glm::mat4 GetLightSpaceMatrix()
            {
                float ortho_size = 200.0f;
                glm::mat4 light_projection = glm::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, 0.1f, 400.0f);

                // Try a fixed light position first to rule out direction issues
                glm::vec3 model_center = glm::vec3(0.0f, 0.0f, 0.0f);

                glm::vec3 light_direction = glm::normalize(directional_light_->direction);
                glm::vec3 light_pos = model_center - light_direction * 300.0f;

                // Fixed light position pointing down from above (for testing)
                glm::vec3 light_target = model_center;
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Standard up vector

                // If light direction is parallel to up vector, use a different up vector
                if (abs(glm::dot(light_direction, up)) > 0.95f)
                {
                    up = glm::vec3(1.0f, 0.0f, 0.0f); // Use right vector instead
                }

                glm::mat4 light_view = glm::lookAt(light_pos, light_target, up);

                return light_projection * light_view;
            }

            void Bind()
            {
                glViewport(0, 0, width_, height_);
                glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
                glClear(GL_DEPTH_BUFFER_BIT);
            }

            void Unbind()
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            void BindTexture()
            {
                glActiveTexture(GL_TEXTURE0 + 5);
                glBindTexture(GL_TEXTURE_2D, depth_map_texture_);
            }

            void SetDirectionalLight(std::shared_ptr<Lighting::DirectionalLight> light)
            {
                directional_light_ = light;
            }

        private:
            uint32_t width_ = 0;
            uint32_t height_ = 0;
            std::shared_ptr<Lighting::DirectionalLight> directional_light_;
            uint32_t fbo_id_;
            uint32_t depth_map_texture_;
        };
    }
}