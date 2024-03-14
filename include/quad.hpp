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

class Quad_mesh
{
  public:
	Quad_mesh();
	~Quad_mesh();

	void draw();

  private:
	static const std::array<glm::vec2, 4> quad_data;
	static GLuint						  shared_vao;
	static GLuint						  shared_vbo;
	static size_t						  use_count;
};