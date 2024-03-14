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
#include "util.hpp"

class Shader
{
  public:
	static Result<Shader, std::string> create_shader(const std::string& vert_src,
													 const std::string& frag_src);

	GLint operator[](const char* uniform_name) { return glGetUniformLocation(*ptr, uniform_name); }

	Shader(const Shader&) = default;
	Shader(Shader&&)	  = default;

	~Shader()
	{
		if (ptr.use_count() == 1) glDeleteShader(*ptr);
	}

	void use() const { glUseProgram(*ptr); }

  private:
	std::shared_ptr<GLuint> ptr;

	Shader(GLuint shader) :
		ptr(new GLuint(shader))
	{}
};