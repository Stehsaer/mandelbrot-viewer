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

#include "common-include.hpp"
#include "framebuffer.hpp"
#include "palette.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "timer.hpp"

#include <chrono>

struct Mandelbrot_coord
{
	glm::dvec2 center = {0.0, 0.0};
	double	   width  = 2;
};

class Logic_handler
{
  public:
	Logic_handler(float content_scale);

	void update(int width, int height);

  private:
	Framebuffer framebuffer;
	Texture2d	mandelbrot_buffer;
	Texture1d	palette_texture;
	Shader		mandelbrot_shader;

	int display_ratio = 1;

	int	  width = 0, height = 0;
	float content_scale = 1;

	Mandelbrot_coord display_coord, manipulate_coord;

	std::vector<Palette> palette_list
		= {{{{{1.0, 0.0, 0.0}, 0.0}, {{0.0, 1.0, 0.0}, 0.33}, {{0.0, 0.0, 1.0}, 0.67}},
			true,
			"Default Rainbow"}};

	int	 manual_max_iter	 = 256;
	bool manual_iter_enabled = false;
	int	 palette_cycle		 = 256;
	int	 palette_size		 = 256;

	// Control

	float big_step_multiplier	= 1.5;
	float small_step_multiplier = 1.1;

	Query_timer timer;
	float		prev_time_elapsed;

	struct
	{
		float status_bar_height = 30.0f;
	} layout;

	std::optional<std::chrono::steady_clock::time_point> update_time
		= std::chrono::steady_clock::now();

	void set_palette(Palette& palette)
	{
		palette.manipulate_texture(palette_texture, palette_size);
	}

	void update_view();
	void render_view();
	void render_imgui();
};