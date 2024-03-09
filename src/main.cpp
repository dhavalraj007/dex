#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_rect.h"
#include <cstdint>
#include <cstring>
#include <format>
#include <type_traits>

#define SDL_MAIN_HANDLED

#include "Cursor.h"
#include "TextData.h"
#include "core/window.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "shader.h"
#include "stb_image_write.h"
#include "texture.h"


int main()
{
    core::Window window;

    window.create(core::WindowProperties());
    window.setClearColor(glm::vec4(0.0f));
    {
        input::Mouse::Init();
        input::Keyboard::Init();

        Shader shader("../src/shaders/FontVertex.glsl", "../src/shaders/FontFragmentShader.glsl");
        Shader cursor_shader("../src/shaders/CursorVertexShader.glsl", "../src/shaders/CursorFragmentShader.glsl");

        TextData textData;
        textData.setText("I gonna be\nking of the pirates.\n");
        textData.updateRenderDataStartingFrom(0);

        auto orthoMat = glm::ortho(0.f, float(window.getProps().w), 0.f, float(window.getProps().h));

        auto modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat,
                                  glm::vec3{0.f, // translation vector
                                            window.getProps().h - textData.lineHeight * textData.fontSize, 0.f});
        modelMat = glm::scale(modelMat, glm::vec3(textData.fontSize, textData.fontSize, 1.0f));

        shader.setUniformInt("ourTexture", textData.font.texture->getTexUnit());
        shader.setUniformMat4("orthoMat", orthoMat);
        shader.setUniformMat4("modelMat", modelMat);
        cursor_shader.setUniformMat4("orthoMat", orthoMat);
        cursor_shader.setUniformMat4("modelMat", modelMat);
        int32_t counter=0;
        cursor_shader.setUniformInt("counter",++counter);
        glm::vec3 cursor_color{1.0, 1.0, 1.0};
        cursor_shader.setUniformFloat3("in_color", cursor_color);

        while (!window.m_ShouldClose)
        {
            auto currEventHandler = [&window, &textData, &shader,&cursor_shader](SDL_Event e) {
                switch (e.type)
                {
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym)
                    {
                    case SDL_KeyCode::SDLK_LEFT: {
                        textData.cursor.backward();
                    }
                    break;
                    case SDL_KeyCode::SDLK_RIGHT: {
                         SDL_Keymod mod = SDL_GetModState();
                        LOG("mod:{}", mod & KMOD_CTRL);
                        textData.cursor.forward();
                    }
                    break;
                    case SDL_KeyCode::SDLK_UP: {
                        textData.cursor.previous();
                    }
                    break;
                    case SDL_KeyCode::SDLK_DOWN: {
                        textData.cursor.next();
                    }
                    break;
                    case SDL_KeyCode::SDLK_RETURN: {
                        textData.insertChar(textData.cursor.getPosInString(), '\n');
                        textData.updateRenderDataStartingFrom(textData.cursor.getPosInString());
                        textData.cursor.next();
                        textData.cursor.gotoLineStart();
                    }
                    break;
                    case SDL_KeyCode::SDLK_BACKSPACE: {
                        textData.deleteChar(textData.cursor.getPosInString() - 1);
                        textData.updateRenderDataStartingFrom(textData.cursor.getPosInString() - 1);
                        textData.cursor.backward();
                    }
                    break;
                    case SDL_KeyCode::SDLK_DELETE: {
                        textData.deleteChar(textData.cursor.getPosInString());
                        textData.updateRenderDataStartingFrom(textData.cursor.getPosInString());
                    }
                    break;
                    default:
                    break;
                    }
                }
                break; // end of case SDL::KEYDOWN
                case SDL_TEXTINPUT:
                            textData.insertChar(textData.cursor.getPosInString(), e.text.text[0]);
                            assert(strlen(e.text.text)==1); // this should be true ig
                            textData.updateRenderDataStartingFrom(textData.cursor.getPosInString());
                            textData.cursor.forward();
                    break;
                case SDL_WINDOWEVENT: {
                    switch (e.window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED: {
                        auto orthoMat = glm::ortho(0.f, float(window.getProps().w), 0.f, float(window.getProps().h));
                        auto modelMat = glm::mat4(1.0f);
                        modelMat = glm::translate(
                            modelMat,
                            glm::vec3{0.f, // translation vector
                                      window.getProps().h - textData.lineHeight * textData.fontSize, 0.f});
                        modelMat = glm::scale(modelMat, glm::vec3(textData.fontSize, textData.fontSize, 1.0f));
                        shader.setUniformMat4("orthoMat", orthoMat);
                        shader.setUniformMat4("modelMat", modelMat);
                        cursor_shader.setUniformMat4("orthoMat", orthoMat);
                        cursor_shader.setUniformMat4("modelMat", modelMat);
                    }
                    break;
                    }
                }
                break;
                }
            };

            // poll window events
            window.pollEvents(currEventHandler);
            // window.pollEvents([](SDL_Event e){
            //     switch(e.type)
            //     {
            //         case SDL_TEXTINPUT:
            //             LOG("SDL_TEXTINPUT {}",e.text.text);
            //             break;
            //     }
            // });

            //      LOG("current textBuffer:{}",textData.textBuffer);
            // LOG("current cursor on {} which is
            // {}",cursor.getPosInString(),textData.textBuffer[cursor.getPosInString()]);

            //per frame updates
            cursor_shader.setUniformInt("counter",++counter);

            window.clearScreen();
            input::Mouse::Update();
            input::Keyboard::Update();

            shader.bind();
            textData.font.texture->bind();
            textData.render();
            cursor_shader.bind();
            textData.cursor.render();
            window.swapbuffers();
        }
    }
    window.shutdown();

    return 0;
}
