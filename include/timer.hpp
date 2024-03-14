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

#include "common-include.hpp"

class Query_timer
{
  public:
	Query_timer() { glGenQueries(1, &timer); }
	Query_timer(const Query_timer&) = delete;
	Query_timer(Query_timer&&)		= delete;

	void				   start() const { glBeginQuery(GL_TIME_ELAPSED, timer); }
	void				   end() const { glEndQuery(GL_TIME_ELAPSED); }
	[[nodiscard]] uint32_t get_ns() const
	{
		uint32_t time;
		glGetQueryObjectuiv(timer, GL_QUERY_RESULT, &time);
		return time;
	}

  private:
	GLuint timer;
};