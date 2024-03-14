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

#include "texture.hpp"

Texture2d::Texture2d()
{
	GLuint texture;
	glGenTextures(1, &texture);
	ptr = std::make_shared<GLuint>(texture);
}

Texture1d::Texture1d()
{
	GLuint texture;
	glGenTextures(1, &texture);
	ptr = std::make_shared<GLuint>(texture);
}

void Texture2d::stream_data(int			width,
							int			height,
							GLint		internal_format,
							GLenum		pixel_format,
							GLenum		data_format,
							const void* data) const
{
	bind();
	glTexImage2D(
		GL_TEXTURE_2D, 0, internal_format, width, height, 0, pixel_format, data_format, data);
}

void Texture1d::stream_data(int			width,
							GLint		internal_format,
							GLenum		pixel_format,
							GLenum		data_format,
							const void* data) const
{
	bind();
	glTexImage1D(GL_TEXTURE_1D, 0, internal_format, width, 0, pixel_format, data_format, data);
}

void Texture2d::set_filter(GLint filter_min, GLint filter_mag) const
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
}

void Texture1d::set_filter(GLint filter_min, GLint filter_mag) const
{
	bind();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filter_mag);
}

void Texture2d::set_wrap(GLint wrap_s, GLint wrap_t) const
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}

void Texture1d::set_wrap(GLint wrap_s) const
{
	bind();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap_s);
}

Texture2d::~Texture2d()
{
	if (ptr.use_count() == 1) glDeleteTextures(1, ptr.get());
}

Texture1d::~Texture1d()
{
	if (ptr.use_count() == 1) glDeleteTextures(1, ptr.get());
}