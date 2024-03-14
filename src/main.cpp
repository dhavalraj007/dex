#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_rect.h"
#include "TextFrame.h"
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

        TextFrame frame(window,0.f, 100.f, 50.f, 100.f);
        TextFrame frame2(window,50.f, 100.f, 50.f, 100.f);
        while (!window.m_ShouldClose)
        {
            auto currEventHandler = [&frame,&frame2](SDL_Event e) {
                frame.handleEvent(e);
                frame2.handleEvent(e);
            };

            // poll window events
            window.pollEvents(currEventHandler);

            //per frame updates
            frame.update();
            frame2.update();

            window.clearScreen();
            input::Mouse::Update();
            input::Keyboard::Update();

            frame.render();
            frame2.render();
            window.swapbuffers();
        }
    }
    window.shutdown();

    return 0;
}
