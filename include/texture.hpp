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

class Texture1d
{
  public:
	Texture1d(const Texture1d&) = delete;
	Texture1d(Texture1d&&)		= default;
	Texture1d();
	~Texture1d();

	void bind() const { glBindTexture(GL_TEXTURE_1D, *ptr); }
	void bind_slot(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		bind();
	}

	void stream_data(int		 width,
					 GLint		 internal_format,
					 GLenum		 pixel_format = GL_RGB,
					 GLenum		 data_format  = GL_UNSIGNED_BYTE,
					 const void* data		  = nullptr) const;

	void set_filter(GLint filter_min, GLint filter_mag) const;
	void set_wrap(GLint wrap_s) const;

  private:
	std::shared_ptr<GLuint> ptr;
};

class Texture2d
{
  public:
	Texture2d(const Texture2d&) = delete;
	Texture2d(Texture2d&&)		= default;
	Texture2d();
	~Texture2d();

	void bind() const { glBindTexture(GL_TEXTURE_2D, *ptr); }
	void bind_slot(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		bind();
	}

	void stream_data(int		 width,
					 int		 height,
					 GLint		 internal_format,
					 GLenum		 pixel_format = GL_RGB,
					 GLenum		 data_format  = GL_UNSIGNED_BYTE,
					 const void* data		  = nullptr) const;

	void set_filter(GLint filter_min, GLint filter_mag) const;
	void set_wrap(GLint wrap_s, GLint wrap_t) const;

	GLuint operator*() const { return *ptr; }

  private:
	std::shared_ptr<GLuint> ptr;
};