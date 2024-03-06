#pragma once
#include "TextData.h"
#include "glm/vec2.hpp"

struct LineInfo
{
    int start;
    int end;
    bool isLast;
    bool isFirst;
    inline int length()
    {
        return end - start + 1;
    }
};
struct Cursor
{
    Cursor(const TextData &);
    void update();
    void render();
    void forward();
    void backward();
    void next();
    void previous();
    void gotoLineStart();
    LineInfo getLineInfo(int textIndex);
    const int &getPosInString()
    {
        return posInString;
    }

  private:
    const TextData &textDataRef;
    uint32_t vao;
    uint32_t vbo;
    glm::vec2 posInPlane = {0, 0};
    int posInString = 0;
    float cursorHeight = 1;
    float cursorWidth = 0.1;
};
