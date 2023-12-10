#include "TextData.h"
#include "Font.h"
#include "glm/ext/matrix_transform.hpp"

//
// Created by Dhava on 18-11-2023.
//

void TextData::render() {
  glBindVertexArray(vaoId);
  glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
}

TextData::TextData() : fontSize(30), scaledLineHeight(0) {
  const int defaultNumVerts = 10000;
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

void TextData::genRenderData(Font &font) {

  // for local space scaling
  const double maxSize = (font.fontGeometry.getMetrics().ascenderY -
                          font.fontGeometry.getMetrics().descenderY);
  const double fontScale = 1.0 / maxSize;

  // for screen translation/scaling (this will be used by model matrix)

  scaledLineHeight =
      (fontScale * fontSize * font.fontGeometry.getMetrics().lineHeight);
  double x = 0;
  double y = 0;

  for (int i = 0; i < textBuffer.size(); i++) {
    char character = textBuffer[i];
    auto geometry = font.fontGeometry.getGlyph(character);

    if (character == '\r' or character == '\n') {
      x = 0;
      y -= fontScale * font.fontGeometry.getMetrics().lineHeight;

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

    // dont add advance for last character
    if (i < textBuffer.size() - 1) {
      double advance = geometry->getAdvance();
      char nextCharacter = textBuffer[i + 1];
      font.fontGeometry.getAdvance(advance, character, nextCharacter);
      x += advance * fontScale;
    }
  }
  glBindVertexArray(vaoId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(TextVertex),
               vertexData.data(), GL_DYNAMIC_DRAW);
  // pos attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void TextData::updateRenderDataStartingFrom(Font &font, uint32_t start) {

  // for local space scaling
  const double maxSize = (font.fontGeometry.getMetrics().ascenderY -
                          font.fontGeometry.getMetrics().descenderY);
  const double fontScale = 1.0 / maxSize;

  // for screen translation/scaling (this will be used by model matrix)
  double fontSize = 0.5;
  scaledLineHeight =
      fontScale * fontSize * font.fontGeometry.getMetrics().lineHeight;

  double x = vertexData.back().pos.x;
  double y = vertexData.back().pos.y;

  int vertexIndex = start * 6;
  vertexData.erase(vertexIndex + vertexData.begin(), vertexData.end());

  //set advance for start and start-1 character
  double advance=0;
  if(start-1>=0)
    font.fontGeometry.getAdvance(advance,textBuffer[start-1],textBuffer[start]);
  x+= advance;
  
  
  for (int i = start; i < textBuffer.size(); i++) {
    char character = textBuffer[i];
    auto geometry = font.fontGeometry.getGlyph(character);

    if (character == '\r' or character == '\n') {
      x = 0;
      y -= fontScale * font.fontGeometry.getMetrics().lineHeight;
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

    // dont add advance for last character
    if (i < textBuffer.size() - 1) {
      double advance = geometry->getAdvance();
      char nextCharacter = textBuffer[i + 1];
      font.fontGeometry.getAdvance(advance, character, nextCharacter);
      x += advance * fontScale;
    }
  }

  glBindVertexArray(vaoId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  LOG("\n{},{},{}\n", vertexIndex ,vertexIndex * sizeof(TextVertex),(vertexData.size()-vertexIndex)*sizeof(TextVertex));
  glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(TextVertex),(vertexData.size()-vertexIndex)*sizeof(TextVertex), &vertexData[vertexIndex]);
  // pos attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}
