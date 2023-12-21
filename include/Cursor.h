#pragma once
#include "glm/vec2.hpp"
#include "TextData.h"

struct LineInfo{
  int start;
  int end;
  bool isLast;
  bool isFirst;
  inline int length(){return end-start+1;}
};
struct Cursor {
  Cursor(const TextData&);
  void update();
  void render();
  void forward();
  void backward();
  void next();
  void previous();
  LineInfo getLineInfo(int textIndex);
  const TextData& textDataRef;
  uint32_t vao;
  uint32_t vbo;
  glm::vec2 posInPlane = {0, 0};
  int posInString = 0;
  float cursorHeight = 1;
  float cursorWidth = 0.1;
};
