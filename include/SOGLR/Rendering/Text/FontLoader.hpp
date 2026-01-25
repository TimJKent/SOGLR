#include <ft2build.h>
#include FT_FREETYPE_H


#include <iostream>
#include <expected>

#include <glad/glad.h>
#include "Rendering/Texture.hpp"
#include "Rendering/Text/Font.hpp"

namespace SOGLR {
namespace Text {
class FontLoader {
public:
   FontLoader() {}
   ~FontLoader(){}

   std::expected<std::shared_ptr<Font>, std::string> LoadFont(const std::string& font_path, int font_size = 48)
   {
       std::shared_ptr<Font> font = std::make_shared<Font>(font_size);
       FT_Library ft;
       if (FT_Init_FreeType(&ft))
       {
            return std::unexpected("ERROR::FREETYPE: Could not init FreeType Library");
       }

       FT_Face face;
       if (FT_New_Face(ft, font_path.c_str(), 0, &face))
       {
           return std::unexpected("ERROR::FREETYPE: Failed to load font:" + font_path);
       }
       FT_Set_Pixel_Sizes(face, 0, font_size);  
       
       glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

       for (unsigned char c = 0; c < 128; c++)
       {
           // load character glyph 
           if (FT_Load_Char(face, c, FT_LOAD_RENDER))
           {
               std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
               continue;
           }

           // generate texture
           std::shared_ptr<Texture> texture = std::make_shared<Texture>(face->glyph->bitmap.width, face->glyph->bitmap.rows, Texture::TEXTURE_TYPE::TEXT, face->glyph->bitmap.buffer);
           
           // now store character for later use
           Character character = {
               texture, 
               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
               face->glyph->advance.x
           };
           font->Characters.insert(std::pair<char, Character>(c, character));
       }

       return font;
   }
private:
    FT_Library ftLibrary;
    FT_Face ftFace;
};

}}