
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

int main() {
  core::Window window;

  window.create(core::WindowProperties());
  window.setClearColor(glm::vec4(0.0f));

  input::Mouse::Init();
  input::Keyboard::Init();

  Shader shader("../src/shaders/FontVertex.glsl",
                "../src/shaders/FontFragmentShader.glsl");

  Font font(R"(C:\Windows\Fonts\verdana.ttf)");

  TextData textData;
  textData.setText("I am gonna be \n king of the pirates.");
  textData.genRenderData(font);

  float width = 16.f;
  auto orthoMat =
      glm::ortho(0.f, width, 0.f, width * (1 / window.getProps().aspectRatio));

  glm::mat4 modelMat = glm::mat4(1.0f);
  modelMat = glm::translate(
      modelMat, glm::vec3{0.f, // translation vector
                          9.0f - textData.getScaledLineHeight(), 0.f});
  modelMat = glm::scale(modelMat,
                        glm::vec3(textData.fontSize, textData.fontSize, 1.0f));

  shader.setUniformInt("ourTexture", font.texture->getTexUnit());
  shader.setUniformMat4("orthoMat", orthoMat);
  shader.setUniformMat4("modelMat", modelMat);

  while (!window.m_ShouldClose) {
    // poll window events
    window.pollEvents([&window, &textData, &font](SDL_Event e) {
      switch (e.type) {
      case SDL_KEYDOWN: {
        if (SDL_KeyCode::SDLK_RETURN <= e.key.keysym.sym and
            e.key.keysym.sym <= SDL_KeyCode::SDLK_z) {
          textData.appendChar(e.key.keysym.sym);// 0 1 2 3 4 5
          textData.updateRenderDataStartingFrom(font,textData.textBuffer.size()-1);
        }
        break;
      }
      }
    });

    window.clearScreen();
    input::Mouse::Update();
    input::Keyboard::Update();

    shader.bind();
    font.texture->bind();
    textData.render();

    window.swapbuffers();
  }
  window.shutdown();

  return 0;
}
