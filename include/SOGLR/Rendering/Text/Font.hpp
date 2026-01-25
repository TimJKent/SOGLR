#pragma once
#include <map>
#include "Character.hpp"

namespace SOGLR::Text
{
    class Font
    {
    public:
        Font(uint32_t size) : size_(size) {}
        ~Font() = default;
        const Character& GetCharacter(char character)
        {
            return Characters.at(character);
        }

        uint32_t GetSize() const
        {
            return size_;
        }
    private:
        std::map<char, Character> Characters;
        uint32_t size_;
    friend class FontLoader;
    };
}