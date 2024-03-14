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

#include "logic.hpp"
#include "quad.hpp"
#include "resources.hpp"

void Logic_handler::update_view()
{
	using namespace std::chrono_literals;
	auto& io = ImGui::GetIO();

	if (!io.WantCaptureMouse)
	{
		auto drag = ImGui::GetMouseDragDelta(0);

		if (drag.x != 0 || drag.y != 0)
		{
			update_time = std::chrono::steady_clock::now() + 200ms;

			double aspect_ratio = (double)width / height;
			double delta_x		= display_coord.width * drag.x / width,
				   delta_y		= display_coord.width / aspect_ratio * drag.y / height;

			manipulate_coord
				= {{display_coord.center.x - delta_x, display_coord.center.y - delta_y},
				   display_coord.width};
		}
		else if (io.MouseWheel != 0.0)
		{
			update_time = std::chrono::steady_clock::now() + 200ms;

			float step
				= ImGui::IsKeyDown(ImGuiKey_ModCtrl) ? small_step_multiplier : big_step_multiplier;

			float mul = pow(0.8f, step * io.MouseWheel);

			auto [mouse_x, mouse_y] = io.MousePos;

			mouse_x = (mouse_x - width * 0.5) / width;
			mouse_y = (mouse_y - height * 0.5) / width;

			glm::dvec2 mouse_center
				= manipulate_coord.center + glm::dvec2(mouse_x, mouse_y) * manipulate_coord.width;

			manipulate_coord.width *= mul;
			manipulate_coord.center
				= mouse_center + (manipulate_coord.center - mouse_center) * (double)mul;
		}

		manipulate_coord.center
			= glm::clamp(manipulate_coord.center, glm::dvec2(-2.0, -1.5), glm::dvec2(0.5, 1.5));
		manipulate_coord.width = glm::clamp(manipulate_coord.width, 0.0, 5.0);
	}
}

void Logic_handler::render_view()
{
	if (update_time.has_value())
	{
		if (std::chrono::steady_clock::now() > update_time)
		{
			update_time = std::nullopt;

			timer.start();

			display_coord = manipulate_coord;

			framebuffer.link(mandelbrot_buffer);
			framebuffer.bind();

			glViewport(0, 0, width / display_ratio, height / display_ratio);

			mandelbrot_shader.use();
			palette_texture.bind_slot(0);

			// compute max iteration, special thanks to devs at mandelbrot.silversky.dev
			// const w_45 = widthInUnits / 4.5;
			// const depth = Math.min(180 - 50 * Math.log(w_45) / Math.log(2), 2000);

			double w_45		 = display_coord.width / 4.5;
			int	   iteration = manual_iter_enabled
								 ? manual_max_iter	// Use manual iteration count
								 : (int)glm::clamp(180 - 50 * log(w_45) / log(2),
												   4.0,
												   2000.0);	 // use auto iteration count

			// setup uniforms
			glUniform1i(mandelbrot_shader["palette"], 0);
			glUniform1i(mandelbrot_shader["palette_cycle"], palette_cycle);
			glUniform2d(
				mandelbrot_shader["center"], display_coord.center.x, display_coord.center.y);
			glUniform2d(mandelbrot_shader["size"],
						display_coord.width,
						display_coord.width * height / width);
			glUniform1i(mandelbrot_shader["max_iter"], iteration);

			logger.log(Logger::Info, "Repainting, iteration={}", iteration);

			Quad_mesh().draw();
			util::check_err("5");
			Framebuffer::unbind();
			timer.end();

			glFlush();
			prev_time_elapsed = timer.get_ns() / 1e6f;
		}
	}

	glm::dmat4 forward_matrix(1.0);

	forward_matrix = glm::translate(forward_matrix, glm::dvec3(manipulate_coord.center, 0.0));
	forward_matrix = glm::scale(forward_matrix, glm::dvec3(manipulate_coord.width / 2));
	forward_matrix = glm::scale(forward_matrix, glm::dvec3(2.0 / width, 2.0 / width, 1));
	forward_matrix = glm::translate(forward_matrix, glm::dvec3(-width / 2, -height / 2, 0));

	glm::dmat4 inverse_matrix = glm::inverse(forward_matrix);

	glm::dvec4 top_left = inverse_matrix
						* glm::dvec4(display_coord.center
										 - glm::dvec2(display_coord.width / 2,
													  display_coord.width * height / width / 2),
									 0,
									 1);
	glm::dvec4 bottom_right = inverse_matrix
							* glm::dvec4(display_coord.center
											 + glm::dvec2(display_coord.width / 2,
														  display_coord.width * height / width / 2),
										 0,
										 1);

	auto* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddImage(reinterpret_cast<ImTextureID>(*mandelbrot_buffer),
						{(float)top_left.x, (float)top_left.y},
						{(float)bottom_right.x, (float)bottom_right.y});
}

void Logic_handler::render_imgui()
{
	ImGui::ShowDemoWindow();

	// Status Bar
	ImGui::SetNextWindowPos({0, (float)height}, ImGuiCond_Always, {0, 1});
	ImGui::SetNextWindowSize({(float)width, layout.status_bar_height * content_scale},
							 ImGuiCond_Always);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	if (ImGui::Begin("Bottom Status Bar",
					 nullptr,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
						 | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Zoom %.3ex", 2.0 / manipulate_coord.width);
		ImGui::SameLine(0.0, 50.0);
		ImGui::Text(
			"%.1fms (%.2fms/MP)", prev_time_elapsed, prev_time_elapsed / width / height * 1e6);
	}
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void Logic_handler::update(int width, int height)
{
	if (width != this->width || height != this->height)
	{
		this->width	 = width;
		this->height = height;

		if (width != 0 && height != 0)
			mandelbrot_buffer.stream_data(width / display_ratio, height / display_ratio, GL_RGB8);

		update_time = std::chrono::steady_clock::now();

		logger.log(
			Logger::Info, "Resized buffer: {}x{}px", width / display_ratio, height / display_ratio);
	}

	update_view();
	render_view();
	render_imgui();
}

Logic_handler::Logic_handler(float content_scale) :
	mandelbrot_shader([]() -> Shader {
		auto result
			= Shader::create_shader(resources::to_string(resources::file_shaders_common_vert_),
									resources::to_string(resources::file_shaders_generator_frag_));
		if (!result.ok())
		{
			logger.log(Logger::Error, "Shader Error:\n{}", result.get_err());
			throw std::runtime_error("Shader Error: " + result.get_err());
		}

		return result.get();
	}()),
	content_scale(content_scale)
{
	mandelbrot_buffer.set_filter(GL_LINEAR, GL_LINEAR);
	mandelbrot_buffer.set_wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	palette_texture.set_filter(GL_LINEAR, GL_LINEAR);
	palette_texture.set_wrap(GL_CLAMP_TO_EDGE);

	set_palette(palette_list[0]);
}