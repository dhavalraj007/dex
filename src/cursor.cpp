#include "Cursor.h"
#include "glad/glad.h"
#include <array>

Cursor::Cursor(const TextData &textData) : textDataRef(textData) {
  glGenVertexArrays(1, &vao);
  CHECK_GL_ERROR;
  glGenBuffers(1, &vbo);
  CHECK_GL_ERROR;
  glBindVertexArray(vao);
  CHECK_GL_ERROR;
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  CHECK_GL_ERROR;

  // upload initial buffer on gpu
  std::array<GLfloat, 2 * 6> vertices = {
      posInPlane.x,
      posInPlane.y,

      posInPlane.x + cursorWidth,
      posInPlane.y,

      posInPlane.x,
      posInPlane.y + cursorHeight,

      posInPlane.x,
      posInPlane.y + cursorHeight,

      posInPlane.x + cursorWidth,
      posInPlane.y,

      posInPlane.x + cursorWidth,
      posInPlane.y + cursorHeight,

  };

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_DYNAMIC_DRAW);
  CHECK_GL_ERROR;
  // Specify the layout of the vertex data
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (void *)0);
  CHECK_GL_ERROR;
  glEnableVertexAttribArray(0);
  CHECK_GL_ERROR;
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  CHECK_GL_ERROR;
  glBindVertexArray(0);
  CHECK_GL_ERROR;
}

void Cursor::update() {
  glBindVertexArray(vao);
  CHECK_GL_ERROR;
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  CHECK_GL_ERROR;
  // Vertex data for the cursor line
  std::array<GLfloat, 2 * 6> vertices = {
      posInPlane.x,
      posInPlane.y,

      posInPlane.x + cursorWidth,
      posInPlane.y,

      posInPlane.x,
      posInPlane.y + cursorHeight,

      posInPlane.x,
      posInPlane.y + cursorHeight,

      posInPlane.x + cursorWidth,
      posInPlane.y,

      posInPlane.x + cursorWidth,
      posInPlane.y + cursorHeight,

  };

  // Copy vertex data to VBO
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat),
                  vertices.data());
  CHECK_GL_ERROR;
  // Unbind VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  CHECK_GL_ERROR;
  glBindVertexArray(0);
  CHECK_GL_ERROR;
}

void Cursor::render() {
  glBindVertexArray(vao);
  CHECK_GL_ERROR;
  glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  CHECK_GL_ERROR;
  glBindVertexArray(0);
  CHECK_GL_ERROR;
}

// assumption: all lines (including last) ends with \n
//  line begins from non \n  character and ends at \n (including \n)
LineInfo Cursor::getLineInfo(int textIndex) {
  LineInfo info;
  int curr = textIndex;
  if (textDataRef.textBuffer[curr] == '\n')
    --curr;
  while (curr >= 0 and textDataRef.textBuffer[curr] != '\n')
    --curr;
  // at this point curr is either at previous \n or at -1
  curr++;
  info.start = curr;

  curr = textIndex;
  while (curr < textDataRef.textBuffer.size() and
         textDataRef.textBuffer[curr] != '\n')
    curr++;
  info.end = curr;

  info.isLast = false;
  if (info.end + 1 >= textDataRef.textBuffer.size())
    info.isLast = true;

  info.isFirst = false;
  if (info.start - 1 <= 0)
    info.isFirst = true;

  return info;
}

void Cursor::next() {
  LineInfo currLineInfo = getLineInfo(posInString);
  int numFrontChars = posInString - currLineInfo.start + 1;
  if (currLineInfo.isLast)
    return;
  LineInfo nextLineInfo = getLineInfo(currLineInfo.end + 1);
  int offset = std::min(numFrontChars, nextLineInfo.length());
  int nextCharIndex = nextLineInfo.start - 1 + offset;
  posInString = nextCharIndex;
  posInPlane = textDataRef.getXYCoordFromBufferIndex(posInString);
  update();
}

void Cursor::previous() {
  LineInfo currLineInfo = getLineInfo(posInString);
  int numFrontChars = posInString - currLineInfo.start + 1;
  if (currLineInfo.isFirst)
    return;
  LineInfo prevLineInfo = getLineInfo(currLineInfo.start - 1);
  int offset = std::min(numFrontChars, prevLineInfo.length());
  int nextCharIndex = prevLineInfo.start - 1 + offset;
  posInString = nextCharIndex;
  posInPlane = textDataRef.getXYCoordFromBufferIndex(posInString);
  update();
}

void Cursor::forward() {
  if (posInString + 1 >= textDataRef.textBuffer.size())
    return;
  posInString++;
  posInPlane = textDataRef.getXYCoordFromBufferIndex(posInString);
  update();
}

void Cursor::backward() {

  if (posInString - 1 < 0)
    return;
  --posInString;
  posInPlane = textDataRef.getXYCoordFromBufferIndex(posInString);
  update();
}
