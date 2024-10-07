#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>

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
	DrawArc(xc, yc, x, y, color);
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
		DrawArc(xc, yc, x, y, color);
	}
}

void Framebuffer::DrawArc(int xc, int yc, int x, int y, const color_t& color) {
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


