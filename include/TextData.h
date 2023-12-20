//
// Created by Dhava on 18-11-2023.
//

#ifndef DEEX_TEXTDATA_H
#define DEEX_TEXTDATA_H

#include "Font.h"
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "helpers.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "texture.h"
#include "Cursor.h"
#include <memory>
#include <string>
#include <vector>

struct TextVertex {
  glm::vec2 pos;
  glm::vec2 texCoords;
};


struct TextData {
  std::string textBuffer;
  std::vector<TextVertex>                    
      vertexData; // cpu side vertex data , maintain consistency with gpu side
  Font font;
  uint32_t vboId{};
  uint32_t vaoId{};
  double fontSize;
  double fontScale;
  double lineHeight;

  TextData();


  void setText(std::string str) { textBuffer = std::move(str); }

  void appendText(const std::string &str) { textBuffer.append(str); }

  void appendChar(char c) { textBuffer.push_back(c); }

  void appendText(const char *str) { textBuffer.append(str); }
  void render() ;


<<<<<<< HEAD
  void render();
  void updateRenderDataStartingFrom(Font &font, uint32_t leftMostUpdated,Cursor &cursor);
private:
  double scaledLineHeight;
=======
>>>>>>> b0ac9f4 (fix)

  void updateRenderDataStartingFrom( uint32_t leftMostUpdated);

};

#endif // DEEX_TEXTDATA_H
