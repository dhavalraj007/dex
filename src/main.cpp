#include <filesystem>
#include <format>
#include <iostream>

#include "glad/glad.h"

#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "TextData.h"
#include "core/window.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdfgen-ext.h"
#include "msdfgen.h"
#include "shader.h"
#include "stb_image_write.h"
#include "texture.h"
#include "Cursor.h"

int main() {

  core::Window window;

  window.create(core::WindowProperties());
  //  window.setClearColor(glm::vec4(0.0f));

  input::Mouse::Init();
  input::Keyboard::Init();

  Shader shader("../src/shaders/FontVertex.glsl",
                "../src/shaders/FontFragmentShader.glsl");
  Shader cursor_shader("../src/shaders/CursorVertexShader.glsl",
                "../src/shaders/CursorFragmentShader.glsl");

<<<<<<< HEAD
  Font font("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf");
   Cursor cursor;
  TextData textData;
  textData.setText("I am gonna be \n king of the pirates.");
  textData.updateRenderDataStartingFrom(font,0,cursor);
=======


  TextData textData;
  textData.setText("I am gonna be \n king of the pirates.");
  textData.updateRenderDataStartingFrom(0);
>>>>>>> b0ac9f4 (fix)

  auto orthoMat = glm::ortho(0.f, float(window.getProps().w), 0.f,
                             float(window.getProps().h));

  auto modelMat = glm::mat4(1.0f);
  modelMat = glm::translate(
      modelMat,
      glm::vec3{0.f, // translation vector
                window.getProps().h - textData.lineHeight*textData.fontSize, 0.f});
  modelMat = glm::scale(modelMat,
                        glm::vec3(textData.fontSize, textData.fontSize, 1.0f));

  shader.setUniformInt("ourTexture", textData.font.texture->getTexUnit());
  shader.setUniformMat4("orthoMat", orthoMat);
  shader.setUniformMat4("modelMat", modelMat);
  cursor_shader.setUniformMat4("orthoMat", orthoMat);
  cursor_shader.setUniformMat4("modelMat", modelMat);
  //cursor_shader.
 

  while (!window.m_ShouldClose) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
      std::cout<<err<<std::endl;
    }
    // poll window events
<<<<<<< HEAD
    window.pollEvents([&window, &textData, &font, &shader,&cursor](SDL_Event e) {
=======
    window.pollEvents([&window, &textData, &shader](SDL_Event e) {
>>>>>>> b0ac9f4 (fix)
      switch (e.type) {
      case SDL_KEYDOWN: {
        if (SDL_KeyCode::SDLK_RETURN <= e.key.keysym.sym and
            e.key.keysym.sym <= SDL_KeyCode::SDLK_z) {
<<<<<<< HEAD
          textData.appendChar(e.key.keysym.sym); // 0 1 2 3 4 5
          textData.updateRenderDataStartingFrom(font,textData.textBuffer.size() - 1,cursor);
          //textData.genRenderData(font);
=======
	  if(e.key.keysym.sym==SDLK_RETURN)
	    textData.appendChar('\n');
	  else
	    textData.appendChar(e.key.keysym.sym); 
          textData.updateRenderDataStartingFrom(textData.textBuffer.size() - 1);
>>>>>>> b0ac9f4 (fix)
        }
        break;
      }
      case SDL_WINDOWEVENT: {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          auto orthoMat = glm::ortho(0.f, float(window.getProps().w), 0.f,
                                     float(window.getProps().h));
          auto modelMat = glm::mat4(1.0f);
          modelMat = glm::translate(
              modelMat,
              glm::vec3{0.f, // translation vector
                        window.getProps().h - textData.lineHeight*textData.fontSize,
                        0.f});
          modelMat = glm::scale(
              modelMat, glm::vec3(textData.fontSize, textData.fontSize, 1.0f));
          shader.setUniformMat4("orthoMat", orthoMat);
          shader.setUniformMat4("modelMat", modelMat);

          break;
        }
        }
        break;
      }
      }
    });
    window.clearScreen();
    input::Mouse::Update();
    input::Keyboard::Update();

<<<<<<< HEAD
    //shader.bind();
    cursor_shader.bind();
    font.texture->bind();
    cursor.render();
    shader.bind();
=======
    shader.bind();
    textData.font.texture->bind();
>>>>>>> b0ac9f4 (fix)
    textData.render();

    window.swapbuffers();
  }
  window.shutdown();

  return 0;
}
