/*
 * Copyright 2024 Hsin-chieh Liu
 *
 * Licensed under a modification version of the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://github.com/Stehsaer/mandelbrot-viewer/blob/main/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "palette.hpp"
#include "util.hpp"
#include <algorithm>

void Palette::sort_points()
{
	std::sort(point_list.begin(), point_list.end());
}

Texture1d Palette::gen_texture(int size)
{
	Texture1d tex;
	manipulate_texture(tex, size);

	return tex;
}

static SRGB_color lerp(
	const LAB_color& col1, const LAB_color& col2, float loc1, float loc2, float current_loc)
{
	return LAB_color::lerp(col1, col2, (current_loc - loc1) / (loc2 - loc1)).to_srgb();
}

static void fill(const Palette_point&	  point1,
				 const Palette_point&	  point2,
				 std::vector<SRGB_color>& pixels,
				 bool					  cycle)
{
	const size_t size = pixels.size();

	int left_margin	 = int(floor(point1.location * (float)(size - 1)));
	int right_margin = int(floor(point2.location * (float)(size - 1)));

	if (left_margin > size || left_margin < 0 || right_margin > size || right_margin < 0) return;

	const auto left_color = point1.color.to_lab(), right_color = point2.color.to_lab();

	if (point1.location > point2.location)
	{
		if (cycle)
		{
			right_margin += (int)size;
		}
		else
		{
			for (int i = 0; i < right_margin; i++) pixels[i] = point1.color;
			for (int i = left_margin; i < size; i++) pixels[i] = point2.color;
			return;
		}
	}

	for (int i = left_margin; i <= right_margin; i++)
	{
		pixels[i % size]
			= LAB_color::lerp(left_color,
							  right_color,
							  (float)(i - left_margin) / (float)(right_margin - left_margin))
				  .to_srgb();
	}
}

std::vector<SRGB_color> Palette::gen_pixels(int size)
{
	if (size <= 1)
	{
		logger.log(Logger::Warning, "Can't generate 1D texture of size {}px", size);
		throw std::runtime_error("Exception at Palette::gen_pixels");
	}

	if (point_list.empty())
	{
		logger.log(Logger::Warning, "Empty Palette");
		throw std::runtime_error("Exception at Palette::gen_pixels");
	}

	sort_points();

	std::vector<SRGB_color> pix;
	pix.resize(size);

	if (point_list.size() == 1)
		for (auto& col : pix) col = point_list[0].color;
	else
		for (int i = 0; i < point_list.size(); i++)
			fill(point_list[i], point_list[(i + 1) % point_list.size()], pix, seamless_repeat);

	return pix;
}

void Palette::manipulate_texture(const Texture1d& tex, int size)
{
	auto pix	  = gen_pixels(size);
	auto uint_pix = SRGB_color::to_byte(pix);

	tex.set_wrap(GL_CLAMP_TO_EDGE);
	tex.set_filter(GL_NEAREST, GL_LINEAR);

	tex.stream_data(size, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, uint_pix.data());
}