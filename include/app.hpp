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
#include "framebuffer.hpp"
#include "logic.hpp"
#include "palette.hpp"
#include "quad.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "util.hpp"

class App
{
  public:
	void run();
	void init();

	~App();

  private:
	/* Initialize Section */
	GLFWwindow* window = nullptr;

	struct
	{
		bool glfw	= false;
		bool window = false;
		bool glew	= false;
		bool imgui	= false;
		bool logic	= false;

		[[nodiscard]] bool success() const { return glfw && window && glew && imgui && logic; }
	} init_success;

	void init_window();
	void init_imgui();

	/* Logic Section */
	Logic_handler* logic;
};