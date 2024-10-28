#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>
#include "Image.h"

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
	if (m_texture == nullptr)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	if (x > m_width || x < 0) return;
	if (y > m_height || y < 0) return;

	m_buffer[x + y * m_width] = color;
}

// This function needs more work
void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	// The idea is that we check each pixel to see if it is a pixel we want to fill
	for (int sx = x; sx < x + w; sx++)
	{
		for (int sy = y; sy < y + h; sy++)
		{
				DrawPoint(sx, sy, color);
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int x2, int x3, int y1, int y2, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawCircle(int xc, int yc, int radius, const color_t& color)
{
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
	DrawOctants(xc, yc, x, y, color);
	while (y >= x)
	{
		// Check for decision parameter
		// Update d, y as needed
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
		// Increment x after updating decision parameter
		x++;

		// Draw the circle using the new coords
		DrawOctants(xc, yc, x, y, color);
	}
}

void Framebuffer::DrawOctants(int xc, int yc, int x, int y, const color_t& color) {
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dy = y2 - y1; // Rise
	int dx = x2 - x1; // Run
	float m = dy / (float)dx; // Slope
	float b = y1 - (m * x1); // y-intercept

	if (dx == 0)
	{
		for (int y = y1; y < y2; y++)
		{
			m_buffer[x1 + y * m_width] = color;
		}
		return;
	}

		if (std::abs(dx) > std::abs(dy))
		{
			for (int x = x1; x <= x2; x++)
			{
				// y = mx + b
				int y = (int)(m * x) + b;
				m_buffer[x + y * m_width] = color;
			}
		}
		else
		{
			for (int y = y1; y < y2; y++)
			{
				// x = (y - b) / m
				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	// Calculate deltas
	int dx = x2 - x1;
	int dy = y2 - y1;

	// Check for steep line
	bool steep = (std::abs(dy) > std::abs(dx));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	// Draw line points
	for (int x = x1, y = y1; x <= x2; x++)
	{
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color)
{
	float dt = 1 / 10.0f;
	for (int i = 0; i < 10; i++)
	{
		float t1 = i * dt;
		int sx1 = Lerp(x1, x2, t1);
		int sy1 = Lerp(y1, y2, t1);

		float t2 = t1 * dt;

		int sx2 = Lerp(x1, x2, t2);
		int sy2 = Lerp(y1, y2, t2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	float dt = 1 / 10.0f;
	float t1 = 0;
	for (int i = 0; i < 10; i++)
	{
		int sx1, sy1;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);

		float t2 = t1 + dt;
		int sx2, sy2;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t2, sx2, sy2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const color_t& color)
{
	// Unfinished
	float dt = 1 / 10.0f;
	float t1 = 0;
	for (int i = 0; i < 10; i++)
	{
		int sx1, sy1;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t1, sx1, sy1);

		float t2 = t1 + dt;
		int sx2, sy2;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t2, sx2, sy2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawImage(int x, int y, const Image& image)
{
	// Check if offscreen
	//if (<look at DrawRect for example, use image width and height>) return;
	
	// Iterate through image y
	for (int iy = 0; iy < image.m_height; iy++)
	{
		// Set screen y (image y + y)
		int sy = y + iy;

		// Check if off-screen, don't draw if off-screen
		if (sy < 0 || sy > m_height) return;

		// Iterate through image x
		for (int ix = 0; ix < image.m_width; ix++)
		{
			// Set screen x (image x + x)
			int sx = x + ix;

			// Check if off-screen, don't draw if off-screen
			if (sx < 0 || sx > m_width) return;

			// Get image pixel color
			color_t color = image.m_buffer[ix + iy * image.m_width];

			// Check alpha, if 0 don't draw
			if (color.a == 0) return;

			// Set buffer to color
			DrawPoint(sx, sy, color);
		}
	}
}



