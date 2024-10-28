#include "PostProcess.h"
#include <algorithm>
#include <cmath>

void PostProcess::Invert(std::vector<color_t>& buffer)
{
	std::for_each(buffer.begin(), buffer.end(), [](auto& color)
		{
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		});
}

void PostProcess::Monochrome(std::vector<color_t>& buffer)
{
	std::for_each(buffer.begin(), buffer.end(), [](auto& color)
		{
			uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

			color.r = average;
			color.g = average;
			color.b = average;
		});
}

void PostProcess::Brightness(std::vector<color_t>& buffer, int brightness)
{
	std::for_each(buffer.begin(), buffer.end(), [brightness](auto& color)
		{
			color.r = static_cast<uint8_t>(Clamp(color.r + brightness, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(color.g + brightness, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(color.b + brightness, 0, 255));
		});
}

void PostProcess::BoxBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;
	int k[3][3] =
	{
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		// % 5 : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		//       1, 2, 3, 4, 0, 1, 2, 3, 4, 0
		int x = i % width;
		int y = i / width;

		// skip if out of range
		if (x < 1 || x + 1 >= width || y < 1 || x + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}

		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
	}

}

void PostProcess::GaussianBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;
	int k[3][3] =
	{
		{ 1, 2, 1},
		{ 2, 4, 2},
		{ 1, 2, 1}
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		// % 5 : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		//       1, 2, 3, 4, 0, 1, 2, 3, 4, 0
		int x = i % width;
		int y = i / width;

		// skip if out of range
		if (x < 1 || x + 1 >= width || y < 1 || x + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}

		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
	}
}

void PostProcess::Sharpen(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;
	int k[3][3] =
	{
		{  0, -1,  0 },    // {  0, -2,  0 },
		{ -1,  5, -1 },    // { -2,  8, -2 },
		{  0, -1,  0 }     // {  0, -2,  0 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		// % 5 : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		//       1, 2, 3, 4, 0, 1, 2, 3, 4, 0
		int x = i % width;
		int y = i / width;

		// skip if out of range
		if (x < 1 || x + 1 >= width || y < 1 || x + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}

		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9); // Clamp{r, 0, 255)
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);

		//color.r = static_cast<uint8_t>(Clamp{r, 0, 255);
		//color.g = static_cast<uint8_t>(Clamp{g, 0, 255);
		//color.b = static_cast<uint8_t>(Clamp{b, 0, 255);
	}
}

void PostProcess::Edge(std::vector<color_t>& buffer, int width, int height, int threshhold)
{
	std::vector<color_t> source = buffer;
	int hk[3][3] =
	{
		{ 1, 0, -1 },
		{ 2, 0, -2 },
		{ 1, 0, -1 } 
	};

	int vk[3][3] =
	{
		{ -1, -2, -1 },
		{  0,  0,  0 },
		{  1,  2,  1 }
	};

	for (int i = 0; i < buffer.size(); i++)
	{
		// % 5 : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		//       1, 2, 3, 4, 0, 1, 2, 3, 4, 0
		int x = i % width;
		int y = i / width;

		// skip if out of range
		if (x < 1 || x + 1 >= width || y < 1 || x + 1 >= height) continue;

		int h = 0;
		int v = 0;

		for (int iy = 0; iy < 3; iy++)
		{
			for (int ix = 0; ix < 3; ix++)
			{
				color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];

				h += pixel.r * hk[iy][ix];
				v += pixel.r * vk[iy][ix];

			}
		}

		int m = static_cast<int>(std::sqrt(h * h + v * v));
		m = (m >= threshhold) ? m : 0;

		uint8_t c = Clamp(m, 0, 255);
		color_t color = buffer[i];

		color.r = c;
		color.g = c;
		color.b = c;
		//color.r = static_cast<uint8_t>(Clamp{r, 0, 255);
		//color.g = static_cast<uint8_t>(Clamp{g, 0, 255);
		//color.b = static_cast<uint8_t>(Clamp{b, 0, 255);
	}
}
