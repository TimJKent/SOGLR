#pragma once

#include <cstdint>
#include <string>

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <istream>
#include <glm/glm.hpp>

namespace SOGLR
{
    class Shader
    {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath)
        {
            uint32_t vertexShader = Compile(vertexPath, GL_VERTEX_SHADER);
            uint32_t fragmentShader = Compile(fragmentPath, GL_FRAGMENT_SHADER);

            if (vertexShader == 0 || fragmentShader == 0)
            {
                return;
            }

            renderer_id_ = glCreateProgram();
            glAttachShader(renderer_id_, vertexShader);
            glAttachShader(renderer_id_, fragmentShader);
            glLinkProgram(renderer_id_);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Set up UBO for camera matrices
            block_index_ = glGetUniformBlockIndex(renderer_id_, "Matrices");
            if (block_index_ != GL_INVALID_INDEX)
            {
                glUniformBlockBinding(renderer_id_, block_index_, 0);

                glGenBuffers(1, &ubo_matrices_);
                glBindBuffer(GL_UNIFORM_BUFFER, ubo_matrices_);
                size_t buffer_size = 2 * 64 + 4 * 16; // 2 mat4 + 4 vec4 (for 3 vec3 + 1 float)
                glBufferData(GL_UNIFORM_BUFFER, buffer_size, NULL, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_matrices_, 0, buffer_size);
            }
            else
            {
                ubo_matrices_ = 0; // Fallback to individual uniforms
            }
        }

        uint32_t Compile(const std::string &shaderPath, uint32_t type)
        {
            std::fstream shaderFile;
            shaderFile.open(shaderPath, std::ios::in);

            if (!shaderFile.is_open())
            {
                std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
                return 0;
            }

            std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
            shaderFile.close();

            const char *shaderCodeCharPtr = shaderCode.c_str();

            uint32_t shader_id = glCreateShader(type);
            glShaderSource(shader_id, 1, &shaderCodeCharPtr, NULL);
            glCompileShader(shader_id);
            int success;
            char infoLog[512];
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            }

            return shader_id;
        }

        ~Shader()
        {
            if (ubo_matrices_ != 0)
            {
                glDeleteBuffers(1, &ubo_matrices_);
            }
            glDeleteProgram(renderer_id_);
        }

        void Bind() const
        {
            glUseProgram(renderer_id_);
        }

        void Unbind() const
        {
            glUseProgram(0);
        }

        void SetUBOMatrices(
            const glm::mat4 &projection,
            const glm::mat4 &view,
            const glm::vec3 &camera_pos,
            const glm::vec3 &directional_light_direction,
            const glm::vec3 &directional_light_color,
            float directional_light_intensity)
        {
            if (block_index_ != GL_INVALID_INDEX && ubo_matrices_ != 0)
            {
                // Use UBO with proper std140 alignment
                glBindBuffer(GL_UNIFORM_BUFFER, ubo_matrices_);

                // std140 offsets: mat4 = 64 bytes, vec3 padded to vec4 = 16 bytes
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
                glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::mat4), glm::value_ptr(view));
                glBufferSubData(GL_UNIFORM_BUFFER, 128, sizeof(glm::vec3), glm::value_ptr(camera_pos));
                glBufferSubData(GL_UNIFORM_BUFFER, 144, sizeof(glm::vec3), glm::value_ptr(directional_light_direction));
                glBufferSubData(GL_UNIFORM_BUFFER, 160, sizeof(glm::vec3), glm::value_ptr(directional_light_color));
                glBufferSubData(GL_UNIFORM_BUFFER, 176, sizeof(float), &directional_light_intensity);

                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }
        }

        void SetUniformMat4f(const char *name, const glm::mat4 &matrix)
        {
            GLint location = glGetUniformLocation(renderer_id_, name);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
        }

        void SetUniform3f(const char *name, const glm::vec3 &vector)
        {
            GLint location = glGetUniformLocation(renderer_id_, name);
            glUniform3f(location, vector.x, vector.y, vector.z);
        }

        void SetUniform1f(const char *name, float value)
        {
            GLint location = glGetUniformLocation(renderer_id_, name);
            glUniform1f(location, value);
        }

        void SetInt(const std::string &name, int value) const
        {
            GLint loc = glGetUniformLocation(renderer_id_, name.c_str());
            if (loc != -1)
                glUniform1i(loc, value);
        }

    private:
        uint32_t renderer_id_;
        uint32_t block_index_;
        uint32_t ubo_matrices_;
    };
} // namespace SOGLR