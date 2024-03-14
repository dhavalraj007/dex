#include "TextFrame.h"
#include "SDL_events.h"
#include "core/window.h"

TextFrame::TextFrame(const core::Window& refWindow) : TextFrame(refWindow,0, 0, 1000, 100)
{}

TextFrame::TextFrame(const core::Window& refWindow,float x, float y, float w, float h)
    : refWindow(refWindow),fontShader("../src/shaders/FontVertex.glsl", "../src/shaders/FontFragmentShader.glsl"),
      cursorShader("../src/shaders/CursorVertexShader.glsl", "../src/shaders/CursorFragmentShader.glsl"),
      cursor(textData)
{
   setPos(x,y);
   setDim(w, h);
   setCursorColor({1.0,1.0,1.0});
   fontShader.setUniformInt("ourTexture",textData.font.texture->getTexUnit());
}

glm::vec2 TextFrame::scaleToWindowCoords(glm::vec2 coord)
{
    return coord * glm::vec2{refWindow.getProps().w,refWindow.getProps().h} * 0.01f;
}

void TextFrame::setPosX(float x)
{
  setPos(x,pos.y);
}

void TextFrame::setPosY(float y)
{
  setPos(pos.x,y);
}

void TextFrame::setPos(float x,float y)
{
    pos = glm::vec2{x,y};
    auto wndwPos = scaleToWindowCoords(pos);
    auto modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat,
                                  glm::vec3{wndwPos.x, // translation vector
                                            wndwPos.y - textData.lineHeight * textData.fontSize, 0.f});
    modelMat = glm::scale(modelMat, glm::vec3(textData.fontSize, textData.fontSize, 1.0f));
    fontShader.setUniformMat4("modelMat", modelMat);
    cursorShader.setUniformMat4("modelMat", modelMat);

    setDim(dim.x, dim.y);
}

void TextFrame::setDimW(float w)
{
  setDim(w,dim.y);
}

void TextFrame::setDimH(float h)
{
  setDim(dim.x,h);
}

void TextFrame::setDim(float w,float h)
{
    dim = glm::vec2{w,h};
    auto wndwDim = scaleToWindowCoords(dim);
    auto wndwPos = scaleToWindowCoords(pos);

    auto orthoMat = glm::ortho(wndwPos.x, wndwPos.x + wndwDim.x, wndwPos.y - wndwDim.y, wndwPos.y);
    fontShader.setUniformMat4("orthoMat", orthoMat);
    cursorShader.setUniformMat4("orthoMat", orthoMat);
}

void TextFrame::setCursorColor(glm::vec3 col)
{
    cursorColor = col;
    cursorShader.setUniformFloat3("in_color", col);
}

void TextFrame::update()
{
    static int32_t counter=0;
    cursorShader.setUniformInt("counter",++counter);
}

void TextFrame::handleEvent(SDL_Event e)
{
   switch (e.type)
   {
   case SDL_KEYDOWN:
       switch (e.key.keysym.sym)
       {
       case SDL_KeyCode::SDLK_LEFT: {
           cursor.backward();
       }
       break;
       case SDL_KeyCode::SDLK_RIGHT: {
           SDL_Keymod mod = SDL_GetModState();
           LOG("mod:{}", mod & KMOD_CTRL);
           cursor.forward();
       }
       break;
       case SDL_KeyCode::SDLK_UP: {
           cursor.previous();
       }
       break;
       case SDL_KeyCode::SDLK_DOWN: {
           cursor.next();
       }
       break;
       case SDL_KeyCode::SDLK_RETURN: {
           textData.insertChar(cursor.getPosInString(), '\n');
           textData.updateRenderDataStartingFrom(cursor.getPosInString());
           cursor.next();
           cursor.gotoLineStart();
       }
       break;
       case SDL_KeyCode::SDLK_BACKSPACE: {
           textData.deleteChar(cursor.getPosInString() - 1);
           textData.updateRenderDataStartingFrom(cursor.getPosInString() - 1);
           cursor.backward();
       }
       break;
       case SDL_KeyCode::SDLK_DELETE: {
           textData.deleteChar(cursor.getPosInString());
           textData.updateRenderDataStartingFrom(cursor.getPosInString());
       }
       break;
       default:
           break;
       }
       break; // end of case SDL::KEYDOWN
   case SDL_TEXTINPUT:
       textData.insertChar(cursor.getPosInString(), e.text.text[0]);
       assert(strlen(e.text.text) == 1); // this should be true ig
       textData.updateRenderDataStartingFrom(cursor.getPosInString());
       cursor.forward();
       break;
   case SDL_WINDOWEVENT:
       switch (e.window.event)
       {
       case SDL_WINDOWEVENT_RESIZED:
       {
           //recalculate dim and pos cause window width,height may have changed
           setPos(pos.x,pos.y);
           setDim(dim.x,dim.y);
       }
           break;
       default:
           break;
       }
       break;
   }
}

void TextFrame::render()
{
    glm::vec2 wndwBotLeftPos = scaleToWindowCoords(pos - glm::vec2{0,dim.y});
    glm::vec2 wndwDim = scaleToWindowCoords(dim);
    glViewport(wndwBotLeftPos.x,wndwBotLeftPos.y,wndwDim.x,wndwDim.y);
    fontShader.bind();
    textData.font.texture->bind();
    textData.render();
    cursorShader.bind();
    cursor.render();
}
