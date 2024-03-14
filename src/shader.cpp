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

#include "shader.hpp"

Result<Shader, std::string> Shader::create_shader(const std::string& vert, const std::string& frag)
{
	GLuint vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertex_cstr	  = vert.c_str();
	const char* fragment_cstr = frag.c_str();

	glShaderSource(vertex_shader, 1, &vertex_cstr, nullptr);
	glCompileShader(vertex_shader);

	GLint success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		return std::string(info_log);
	}

	glShaderSource(fragment_shader, 1, &fragment_cstr, nullptr);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		return std::string(info_log);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info_log[512];
		glGetProgramInfoLog(program, 512, nullptr, info_log);
		return std::string(info_log);
	}

	return Shader(program);
}