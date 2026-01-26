#pragma once

#include <variant>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace SOGLR
{
    class Material
    {
    public:
        Material() = default;
        ~Material() = default;

        void SetProperty(const std::string &name, const std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4> &value)
        {
            properties_[name] = value;
        }

        const std::unordered_map<std::string, std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>> &GetProperties() const
        {
            return properties_;
        }

    private:
        std::unordered_map<std::string, std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>> properties_;
    };
}