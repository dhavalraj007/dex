#pragma once
#include "glm/vec2.hpp"
class Cursor{

    public:
    uint32_t vao;
    uint32_t vbo;
    glm::vec2 posInPlane={0,0};
    int posInString=0;
    float cursorHeight=0.4;
    float cursorWidth =0.1;
    void render();
};