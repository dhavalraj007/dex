#include "TextData.h"
#include "Font.h"
#include "glm/ext/matrix_transform.hpp"

<<<<<<< HEAD

//
// Created by Dhava on 18-11-2023.
//

=======
>>>>>>> b0ac9f4 (fix)
void TextData::render() {
  glBindVertexArray(vaoId);
  glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
}

TextData::TextData() : fontSize(30), font("../MouldyCheeseRegular-WyMWG.ttf") {
  const double maxSize = (font.fontGeometry.getMetrics().ascenderY -
                          font.fontGeometry.getMetrics().descenderY);
  fontScale = 1.0 / maxSize;
  lineHeight = fontScale * font.fontGeometry.getMetrics().lineHeight;

  const int defaultNumVerts = 300;
  vertexData.reserve(defaultNumVerts);
  glGenBuffers(1, &vboId);
  CHECK_GL_ERROR;
  glGenVertexArrays(1, &vaoId);
  CHECK_GL_ERROR;

  glBindVertexArray(vaoId);
  CHECK_GL_ERROR;

  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  CHECK_GL_ERROR;
  glBufferData(GL_ARRAY_BUFFER, defaultNumVerts * sizeof(TextVertex), nullptr,
               GL_DYNAMIC_DRAW);
  CHECK_GL_ERROR;
}

<<<<<<< HEAD
void TextData::updateRenderDataStartingFrom(Font &font, uint32_t start,Cursor &cursor) {
=======
void TextData::updateRenderDataStartingFrom(uint32_t start) {
>>>>>>> b0ac9f4 (fix)

  // for local space scaling
  const double maxSize = (font.fontGeometry.getMetrics().ascenderY -
                          font.fontGeometry.getMetrics().descenderY);
  const double fontScale = 1.0 / maxSize;

<<<<<<< HEAD
  // for screen translation/scaling (this will be used by model matrix)
  //double fontSize = 0.5;
  scaledLineHeight =
      fontScale * fontSize * font.fontGeometry.getMetrics().lineHeight;
  double x=cursor.posInPlane.x;
  double y=cursor.posInPlane.y;
=======
>>>>>>> b0ac9f4 (fix)
  int vertexIndex = start * 6;
  //case when last entered char is \n 

  vertexData.erase(vertexIndex + vertexData.begin(), vertexData.end());
<<<<<<< HEAD
  
=======

  double x = 0;
  double y = 0;
  // getting x and y from start-1 char's right top
  if (start >= 1) {
    if (textBuffer[start - 1] == '\n') {
      int prev = start - 1;
      int count = 0;
      while (prev > 0 && textBuffer[prev] == '\n') {
        count++;
        prev--;
      }

      auto geo = font.fontGeometry.getGlyph(textBuffer[prev]);
      double temp_pl, temp_pb, temp_pr, temp_pt;
      geo->getQuadPlaneBounds(temp_pl, temp_pb, temp_pr, temp_pt);
      int vertexPrevIndex = prev * 6;
      y = vertexData[vertexPrevIndex + 5].pos.y - (temp_pt * fontScale) -
          (count * lineHeight);
    } else {
      auto geo = font.fontGeometry.getGlyph(textBuffer[start - 1]);
      double temp_pl, temp_pb, temp_pr, temp_pt;
      geo->getQuadPlaneBounds(temp_pl, temp_pb, temp_pr, temp_pt);
      x = vertexData.back().pos.x - (temp_pr * fontScale);
      y = vertexData.back().pos.y - (temp_pt * fontScale);
    }
  }

  // set advance for start and start-1 character
  double advance = 0;
  if (start >= 1) {
    font.fontGeometry.getAdvance(advance, textBuffer[start - 1],
                                 textBuffer[start]);
  }
  x += fontScale * advance;

>>>>>>> b0ac9f4 (fix)
  for (int i = start; i < textBuffer.size(); i++) {
    char character = textBuffer[i];
    auto geometry = font.fontGeometry.getGlyph(character);

    if (character == '\r' or character == '\n') {

      TextVertex vertexDontDraw{
          glm::vec2(std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::infinity()),
          glm::vec2(std::numeric_limits<float>::infinity(),
                    std::numeric_limits<float>::infinity())};
      vertexData.push_back(vertexDontDraw);
      vertexData.push_back(vertexDontDraw);
      vertexData.push_back(vertexDontDraw);
      vertexData.push_back(vertexDontDraw);
      vertexData.push_back(vertexDontDraw);
      vertexData.push_back(vertexDontDraw);

      x = 0;
<<<<<<< HEAD
      y -= fontScale * font.fontGeometry.getMetrics().lineHeight;
      cursor.posInPlane.x=x;
      cursor.posInPlane.y=y;
=======
      y -= lineHeight;
>>>>>>> b0ac9f4 (fix)
      continue;
    }

    double al, ab, ar, at;
    geometry->getQuadAtlasBounds(al, ab, ar, at);
    // scale to 0..1
    al *= (1.0 / font.texture->specs.Width);
    ab *= (1.0 / font.texture->specs.Height);
    ar *= (1.0 / font.texture->specs.Width);
    at *= (1.0 / font.texture->specs.Height);

    double pl, pb, pr, pt;
    geometry->getQuadPlaneBounds(pl, pb, pr, pt);
    pl *= fontScale;
    pb *= fontScale;
    pr *= fontScale;
    pt *= fontScale;
    pl += x;
    pr += x;
    pb += y;
    pt += y;
    
    TextVertex vertexLB{glm::vec2(pl, pb), glm::vec2(al, ab)};
    TextVertex vertexRB{glm::vec2(pr, pb), glm::vec2(ar, ab)};
    TextVertex vertexLT{glm::vec2(pl, pt), glm::vec2(al, at)};
    TextVertex vertexRT{glm::vec2(pr, pt), glm::vec2(ar, at)};
    vertexData.push_back(vertexLB);
    vertexData.push_back(vertexRB);
    vertexData.push_back(vertexLT);
    vertexData.push_back(vertexLT);
    vertexData.push_back(vertexRB);
    vertexData.push_back(vertexRT);

    
    double advance = geometry->getAdvance();
    char nextCharacter = textBuffer[i + 1];
    font.fontGeometry.getAdvance(advance, character, nextCharacter);
    x += advance * fontScale;
    
    cursor.posInPlane.x=x;
    cursor.posInPlane.y=y;
  }

  glBindVertexArray(vaoId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  // re-allocate memory according to vertexData's capacity
  GLint buffer_size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);
  if (buffer_size >= sizeof(TextVertex) * vertexData.capacity()) {
    glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(TextVertex),
                    (vertexData.size() - vertexIndex) * sizeof(TextVertex),
                    &vertexData[vertexIndex]);
  } else {
    glBufferData(GL_ARRAY_BUFFER, vertexData.capacity() * sizeof(TextVertex),
                 vertexData.data(), GL_DYNAMIC_DRAW);
  }

  // pos attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}
