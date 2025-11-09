#pragma once

#include <cstdint>

#include <glad/glad.h>

#include "Rendering/Texture.hpp"

namespace SOGLR
{
    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width, uint32_t height)
            : color_texture_(width, height, Texture::DIFFUSE_RGB), width_(0), height_(0)
        {
            glGenFramebuffers(1, &renderer_id_);

            Invalidate(width, height);
        }

        ~Framebuffer()
        {
            glDeleteFramebuffers(1, &renderer_id_);
        }

        void Bind()
        {
            glViewport(0, 0, width_, height_);
            glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void BindTexture()
        {
            color_texture_.Bind();
        }

        void Unbind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Invalidate(uint32_t width, uint32_t height)
        {

            if (width_ == width && height_ == height)
            {
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

            width_ = width;
            height_ = height;

            if (render_buffer_object_id_ != 0)
            {
                glDeleteRenderbuffers(1, &render_buffer_object_id_);
            }

            color_texture_.Resize(width_, height_);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_.GetRendererID(), 0);

            glGenRenderbuffers(1, &render_buffer_object_id_);
            glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_id_);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_object_id_);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        uint32_t GetColorAttachment() const;

    private:
        uint32_t renderer_id_ = 0;
        Texture color_texture_;
        uint32_t render_buffer_object_id_ = 0;
        uint32_t width_ = 0;
        uint32_t height_ = 0;
    };
}