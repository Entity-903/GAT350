#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Framebuffer framebuffer(renderer, 800, 600);

    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        // Clear screen
        
        //SDL_SetRenderDrawColor(renderer.m_renderer, 0, 0, 0, 0);
        //SDL_RenderClear(renderer.m_renderer);

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });
        for (int i = 0; i < 100; i++)
        {
            int x = rand() % 800;
            int y = rand() % 600;
            framebuffer.DrawPoint(x, y, { 255, 255, 255, 255 });
        }

        //int x1 = rand() % framebuffer.m_width;
        //int x2 = rand() % framebuffer.m_width;
        //int x3 = rand() % framebuffer.m_width;
        //int y1 = rand() % framebuffer.m_height;
        //int y2 = rand() % framebuffer.m_height;
        //int y3 = rand() % framebuffer.m_height;
        //
        //framebuffer.DrawLine(x1, y1, x2, y2, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(100, 100, 250, 250, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(200, 200, 250, 250, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(300, 300, 250, 250, { 0, 255, 255, 0 });
        //
        //framebuffer.DrawTriangle(50, 50, 200, 200, 50, 200, {0, 255, 255, 0});

        framebuffer.Update();

        renderer.CopyFrameBuffer(framebuffer);

        // Show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}