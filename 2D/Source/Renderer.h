#pragma once
#include <SDL.h>

class Renderer
{
public:
    Renderer() = default;


    friend class Framebuffer;
    friend class Main;

    void Initialize(); // bool
    void CreateWindow(); // bool
    void CopyFrameBuffer(const Framebuffer& framebuffer);

public:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };

public:
    int m_width{ 0 };
    int m_height{ 0 };

};