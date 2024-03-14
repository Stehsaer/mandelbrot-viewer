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

#pragma once

#include "color.hpp"
#include "common-include.hpp"
#include "texture.hpp"

struct Palette_point
{
	SRGB_color color;
	float	   location;

	std::partial_ordering operator<=>(const Palette_point& p) const
	{
		return location <=> p.location;
	}
};

struct Palette
{
	std::vector<Palette_point> point_list;
	bool					   seamless_repeat;
	std::string				   name;

	void					sort_points();
	[[nodiscard]] Texture1d gen_texture(int size);
	void					manipulate_texture(const Texture1d& tex, int size);
	std::vector<SRGB_color> gen_pixels(int size);
};