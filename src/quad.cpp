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

#include "quad.hpp"

/* STATIC MEMBERS */

size_t						   Quad_mesh::use_count	 = 0;
GLuint						   Quad_mesh::shared_vao = 0, Quad_mesh::shared_vbo = 0;
const std::array<glm::vec2, 4> Quad_mesh::quad_data
	= {glm::vec2{-1.0, -1.0}, glm::vec2{1.0, -1.0}, glm::vec2{1.0, 1.0}, glm::vec2{-1.0, 1.0}};

Quad_mesh::Quad_mesh()
{
	if (use_count == 0)
	{
		glGenVertexArrays(1, &shared_vao);
		glBindVertexArray(shared_vao);

		glGenBuffers(1, &shared_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, shared_vbo);

		glBufferData(GL_ARRAY_BUFFER,
					 sizeof(glm::vec2) * quad_data.size(),
					 quad_data.data(),
					 GL_STATIC_DRAW);

		// Assign attribute to shared_vao
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	use_count++;
}

Quad_mesh::~Quad_mesh()
{
	use_count--;

	if (use_count == 0)
	{
		glDeleteVertexArrays(1, &shared_vao);
		glDeleteBuffers(1, &shared_vbo);
	}
}

void Quad_mesh::draw()
{
	glBindVertexArray(shared_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}