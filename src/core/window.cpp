#include "core/window.h"
#include <SDL2/SDL.h>
#include <format>
#include <functional>
#include <glad/glad.h>
#include <iostream>

namespace core
{
    WindowProperties::WindowProperties()
    {
        title = "My Window!";
        x = SDL_WINDOWPOS_CENTERED;
        y = SDL_WINDOWPOS_CENTERED;
        aspectRatio = 16.0f / 9.0f;
        w = 1800;
        h = w * (1 / aspectRatio);
        wMin = 320;
        hMin = wMin * (1 / aspectRatio);
        flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        clearColor = glm::vec4(static_cast<float>(0x64) / static_cast<float>(0xFF),
                               static_cast<float>(0x95) / static_cast<float>(0xFF),
                               static_cast<float>(0xED) / static_cast<float>(0xFF), 1.0f);
    }
    Window::Window() : m_Window(nullptr), m_Context(nullptr)
    {
    }

    void Window::create(const WindowProperties &props)
    {
        m_props = props;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "error init SDL" << std::endl;
        }

        m_Window = SDL_CreateWindow(props.title.c_str(), props.x, props.y, props.w, props.h, props.flags);
        if (!m_Window)
        {
            std::cout << "error window creation" << std::endl;
        }
        SDL_SetWindowMinimumSize(m_Window, props.wMin, props.hMin);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        m_Context = SDL_GL_CreateContext(m_Window);

        gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

        std::cout << std::format("vendor:{} , \nRenderer:{},\nVersion:{} \nShading Language Version:{}",
                                 (const char *)glGetString(GL_VENDOR), (const char *)glGetString(GL_RENDERER),
                                 (const char *)glGetString(GL_VERSION),
                                 (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
        // std::cout<<std::format("{}",(const char*)glGetString(GL_VERSION));

        glViewport(0, 0, props.w, props.h);
        glClearColor(props.clearColor.x, props.clearColor.y, props.clearColor.z, props.clearColor.w);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void Window::shutdown()
    {
        SDL_DestroyWindow(m_Window);
        SDL_GL_DeleteContext(m_Context);
    }

    void Window::setClearColor(glm::vec4 cc)
    {
        m_props.clearColor = cc;
        glClearColor(cc.x, cc.y, cc.z, cc.w);
    }

    void Window::pollEvents(const std::function<void(SDL_Event)> &clientEventHandler)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_ShouldClose = true;
                }
                break;
            case SDL_QUIT:
                m_ShouldClose = true;
                break;
            case SDL_WINDOWEVENT:
                switch (e.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED: {
                    SDL_GetWindowSize(m_Window, &m_props.w, &m_props.h);
                    m_props.aspectRatio = float(m_props.w) / m_props.h;
                    glViewport(0, 0, m_props.w, m_props.h);
                    break;
                }
                default:
                    break;
                }
                break;
            default:
                break;
            }
            clientEventHandler(e);
        }
    }

    void Window::clearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::swapbuffers()
    {
        SDL_GL_SwapWindow(m_Window);
    }

} // namespace core
