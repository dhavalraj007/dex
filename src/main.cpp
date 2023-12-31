#include <filesystem>
#include <format>
#include <iostream>

#include "glad/glad.h"

#define SDL_MAIN_HANDLED

#include "Cursor.h"
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
  {
    input::Mouse::Init();
    input::Keyboard::Init();

    Shader shader("../src/shaders/FontVertex.glsl",
                  "../src/shaders/FontFragmentShader.glsl");
    Shader cursor_shader("../src/shaders/CursorVertexShader.glsl",
                         "../src/shaders/CursorFragmentShader.glsl");

    TextData textData;
    Cursor cursor(textData);
    textData.setText("I am gonna be \n king of the pirates.\n");
    textData.updateRenderDataStartingFrom(0);

    auto orthoMat = glm::ortho(0.f, float(window.getProps().w), 0.f,
                               float(window.getProps().h));

    auto modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(
        modelMat,
        glm::vec3{0.f, // translation vector
                  window.getProps().h - textData.lineHeight * textData.fontSize,
                  0.f});
    modelMat = glm::scale(
        modelMat, glm::vec3(textData.fontSize, textData.fontSize, 1.0f));

    shader.setUniformInt("ourTexture", textData.font.texture->getTexUnit());
    shader.setUniformMat4("orthoMat", orthoMat);
    shader.setUniformMat4("modelMat", modelMat);
    cursor_shader.setUniformMat4("orthoMat", orthoMat);
    cursor_shader.setUniformMat4("modelMat", modelMat);
    glm::vec3 cursor_color{1.0, 1.0, 1.0};
    cursor_shader.setUniformFloat3("in_color", cursor_color);
    while (!window.m_ShouldClose) {

      // poll window events
      window.pollEvents([&window, &textData, &shader, &cursor,
                         &cursor_shader](SDL_Event e) {
        switch (e.type) {
        case SDL_KEYDOWN: {
          switch (e.key.keysym.sym) {
          case SDL_KeyCode::SDLK_LEFT:
            cursor.backward();
            break;
          case SDL_KeyCode::SDLK_RIGHT:
            cursor.forward();
            break;
          case SDL_KeyCode::SDLK_UP:
            cursor.previous();
            break;
          case SDL_KeyCode::SDLK_DOWN:
            cursor.next();
            break;

          default: // printable character pressed?
            if (SDL_KeyCode::SDLK_RETURN <= e.key.keysym.sym and
                e.key.keysym.sym <= SDL_KeyCode::SDLK_z) {
              if (e.key.keysym.sym == SDLK_RETURN) {
                textData.insertChar(cursor.posInString, '\n');
                // donot reorder below lines because cursor.next modifies
                // cursor.posInString
                textData.updateRenderDataStartingFrom(cursor.posInString);
                cursor.next();
              } else {
                if (e.key.keysym.sym >= 0x20 and
                    e.key.keysym.sym < 0x7F) // if its printable ASCII
                  textData.insertChar(cursor.posInString, e.key.keysym.sym);
                // donot reorder below lines because cursor.forward modifies
                // cursor.posInString
                textData.updateRenderDataStartingFrom(cursor.posInString);
                cursor.forward();
              }
            }

            break;
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
                          window.getProps().h -
                              textData.lineHeight * textData.fontSize,
                          0.f});
            modelMat = glm::scale(modelMat, glm::vec3(textData.fontSize,
                                                      textData.fontSize, 1.0f));
            shader.setUniformMat4("orthoMat", orthoMat);
            shader.setUniformMat4("modelMat", modelMat);
            cursor_shader.setUniformMat4("orthoMat", orthoMat);
            cursor_shader.setUniformMat4("modelMat", modelMat);

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

      shader.bind();
      textData.font.texture->bind();
      textData.render();
      cursor_shader.bind();
      cursor.render();
      window.swapbuffers();
    }
  }
  window.shutdown();

  return 0;
}
