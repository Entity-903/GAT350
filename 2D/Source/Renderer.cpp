#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>
#include <SDL.h>

void Renderer::Initialize()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }
}

void Renderer::CreateWindow()
{
    // create window
    // returns pointer to window if successful or nullptr if failed
    m_window = SDL_CreateWindow("Game Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if (m_renderer == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

}
    void Renderer::CopyFrameBuffer(const Framebuffer& framebuffer)
    {
        SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
    }
