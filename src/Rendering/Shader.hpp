#pragma once

#include <cstdint>
#include <string>

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
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            };

            return shader_id;
        }

        ~Shader()
        {
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
    };
} // namespace Rendering