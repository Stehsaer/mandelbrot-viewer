#include <palette.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main()
{
	const int width = 64;

	Palette palette;
	palette.point_list = {{{1.0, 0.0, 0.0}, 0.0}, {{0.0, 1.0, 0.0}, 0.5}, {{0.0, 0.0, 1.0}, 1.0}};
	auto pix		   = palette.gen_pixels(width);
	auto uint_pix	   = SRGB_color::to_byte(pix);

	stbi_write_png("output.png", width, 1, 3, uint_pix.data(), 0);
	system("output.png");
	return 0;
}