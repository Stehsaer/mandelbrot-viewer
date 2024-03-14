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
#include "texture.hpp"

class Framebuffer
{
  public:
	Framebuffer();
	~Framebuffer();

	Framebuffer(const Framebuffer&) = default;
	Framebuffer(Framebuffer&&)		= default;

	void		bind() const { glBindFramebuffer(GL_FRAMEBUFFER, *ptr); }
	static void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void link(const Texture2d& tex) const
	{
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);
	}

	[[nodiscard]] bool is_complete() const
	{
		bind();
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}

  private:
	std::shared_ptr<GLuint> ptr = nullptr;
};