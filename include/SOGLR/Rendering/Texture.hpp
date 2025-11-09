#pragma once
#include <cstdint>

#include <glad/glad.h>
#include "stb_image.hpp"
#include <iostream>

namespace SOGLR
{
    class Texture
    {
    public:
        enum TEXTURE_TYPE
        {
            DIFFUSE_RGB,
            DIFFUSE_RGBA,
            NORMAL,
            SPECULAR,
            HEIGHT,
            DEPTH,
            STENCIL
        };

        constexpr static uint32_t TextureTypeToGLInternalFormat(const TEXTURE_TYPE texture_type)
        {
            switch (texture_type)
            {
            case DEPTH:
                return GL_DEPTH_COMPONENT;
            case STENCIL:
                return GL_DEPTH_STENCIL;
            case DIFFUSE_RGBA:
                return GL_RGBA;
            default:
                return GL_RGB;
            }
        }

        constexpr static uint32_t TextureTypeToImageSlot(const TEXTURE_TYPE texture_type)
        {
            switch (texture_type)
            {
            case DIFFUSE_RGB:
            case DIFFUSE_RGBA:
                return GL_TEXTURE0;
            case NORMAL:
                return GL_TEXTURE1;
            case SPECULAR:
                return GL_TEXTURE2;
            case HEIGHT:
                return GL_TEXTURE3;
            case DEPTH:
                return GL_TEXTURE4;
            case STENCIL:
                return GL_TEXTURE5;
            default:
                return GL_TEXTURE0;
            }
        }

        Texture(const uint32_t width, const uint32_t height, const TEXTURE_TYPE texture_type)
            : texture_type_(texture_type)
        {
            glGenTextures(1, &renderer_id_);
            glBindTexture(GL_TEXTURE_2D, renderer_id_);
            uint32_t format = TextureTypeToGLInternalFormat(texture_type);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Resize(const uint32_t width, const uint32_t height)
        {
            glBindTexture(GL_TEXTURE_2D, renderer_id_);
            glTexImage2D(GL_TEXTURE_2D, 0, TextureTypeToGLInternalFormat(texture_type_), width, height, 0, TextureTypeToGLInternalFormat(texture_type_), GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture(const std::string &path, const TEXTURE_TYPE texture_type = DIFFUSE_RGBA)
            : texture_type_(texture_type)
        {
            glGenTextures(1, &renderer_id_);
            glBindTexture(GL_TEXTURE_2D, renderer_id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            int width, height, nrChannels;
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                GLenum format;
                if (nrChannels == 1)
                    format = GL_RED;
                else if (nrChannels == 3)
                    format = GL_RGB;
                else if (nrChannels == 4)
                    format = GL_RGBA;
                else
                {
                    std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
                    stbi_image_free(data);
                    return;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                // Check for OpenGL errors
                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                {
                    std::cout << "OpenGL error after glTexImage2D: " << error << std::endl;
                }

                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture: " << path << std::endl;
            }
            stbi_image_free(data);
            Unbind();
        }

        ~Texture()
        {
            glDeleteTextures(1, &renderer_id_);
        }

        void Bind() const
        {
            glActiveTexture(TextureTypeToImageSlot(texture_type_));
            glBindTexture(GL_TEXTURE_2D, renderer_id_);
        }

        void Bind(int slot) const
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, renderer_id_);
        }

        void Unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        uint32_t GetRendererID()
        {
            return renderer_id_;
        }

    private:
        uint32_t renderer_id_;
        TEXTURE_TYPE texture_type_;
    };
}