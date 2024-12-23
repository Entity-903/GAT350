#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Framebuffer framebuffer(renderer, 800, 600);

    bool quit = false;
    Image image;
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
        
        //framebuffer.DrawLine(x1, y1, x2, y2, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(100, 100, 250, 250, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(200, 200, 250, 250, { 0, 255, 255, 0 });
        //framebuffer.DrawRect(300, 300, 250, 250, { 0, 255, 255, 0 });
        
        //framebuffer.DrawTriangle(50, 50, 200, 200, 50, 200, { 0, 255, 255, 0 });

        //framebuffer.DrawCircle(300, 300, 50, { 0, 255, 255, 0 });

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255, 255, 0, 255});
        //framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255, 255, 0, 255 });
        //framebuffer.DrawCubicCurve(300, 400, 300, 100, mx, my, 600, 400, { 255, 255, 0, 255 });

        
        image.Load("GenericScene.jpg");
        framebuffer.DrawImage(100, 100, image);
        //PostProcess::Invert(framebuffer.m_buffer);
        //PostProcess::Monochrome(framebuffer.m_buffer);
        //PostProcess::ColorBalance(framebuffer.m_buffer, 155, -50, -50);
        //PostProcess::Brightness(framebuffer.m_buffer, 100);
        //PostProcess::Noise(framebuffer.m_buffer, 80);
        //PostProcess::Threshold(framebuffer.m_buffer, 150);
        //PostProcess::Posterize(framebuffer.m_buffer, 6);
        
        //PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
        //PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 100);
        PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);


        framebuffer.Update();

        renderer.CopyFrameBuffer(framebuffer);

        // Show screen
        // renderer = framebuffer;
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}