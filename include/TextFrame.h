#ifndef TEXTFRAME_H_
#define TEXTFRAME_H_


#include "TextData.h"
#include "core/window.h"
#include "glm/ext/vector_float2.hpp"
#include "shader.h"
union SDL_Event;
struct TextFrame
{
    TextData textData;
    Shader fontShader;
    Shader cursorShader;
    glm::vec2 pos;  // in percent of current window dimension
    glm::vec2 dim;  // in percent of current window dimension
    const core::Window& refWindow;
    glm::vec3 cursorColor;
    Cursor cursor;

    TextFrame(const core::Window& refWindow);
    TextFrame(const core::Window& refWindow,float x,float y,float w,float h);
    glm::vec2 scaleToWindowCoords(glm::vec2 coord);
    void setPosX(float x);
    void setPosY(float y);
    void setPos(float x,float y);
    void setDimW(float w);
    void setDimH(float h);
    void setDim(float w,float h);
    void setCursorColor(glm::vec3 col);
    void update();
    void handleEvent(SDL_Event e);
    void render();
};


#endif // TEXTFRAME_H_
