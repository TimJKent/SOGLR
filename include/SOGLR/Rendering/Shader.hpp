#pragma once

#include <cstdint>
#include <string>

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <istream>
#include <glm/glm.hpp>

namespace Rendering
{
    class Shader
    {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath)
        {
            uint32_t vertexShader = Compile(vertexPath, GL_VERTEX_SHADER);
            uint32_t fragmentShader = Compile(fragmentPath, GL_FRAGMENT_SHADER);

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
                glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 1 * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_matrices_, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
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
                throw std::runtime_error("Failed to open shader file: " + shaderPath);
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

        void SetCameraMatrices(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &camera_pos)
        {
            if (block_index_ != GL_INVALID_INDEX && ubo_matrices_ != 0)
            {
                // Use UBO
                glBindBuffer(GL_UNIFORM_BUFFER, ubo_matrices_);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
                glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
                glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec3), glm::value_ptr(camera_pos));
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }
        }

        void SetUniformMat4f(const char *name, const glm::mat4 &matrix)
        {
            GLint location = glGetUniformLocation(renderer_id_, name);
            glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
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

    private:
        uint32_t renderer_id_;
        uint32_t block_index_;
        uint32_t ubo_matrices_;
    };
} // namespace Rendering