#pragma once
#include <map>
#include "Character.hpp"

namespace SOGLR::Text
{
    class Font
    {
    public:
        const Character& GetCharacter(char character)
        {
            return Characters.at(character);
        }
    private:
        std::map<char, Character> Characters;
    friend class FontLoader;
    };
}