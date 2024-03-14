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

#include "app.hpp"

static const int		 gl_major	= 4;
static const int		 gl_minor	= 3;
static const char* const gl_version = "#version 420";

void App::init()
{
	logger.log(Logger::Info, "Initializing Application");

	try
	{
		init_window();
		init_imgui();

		logic			   = new Logic_handler(util::get_gui_scale(window));
		init_success.logic = true;
	}
	catch (std::exception e)
	{
		logger.log(Logger::Error, "Initialization Error: {}", e.what());
		return;
	}

	logger.log(Logger::Info, "Initializing Done");
}

void App::init_window()
{
	// Initalize GLFW
	if (glfwInit() != GLFW_TRUE)
	{
		const char* err_info;
		glfwGetError(&err_info);

		throw std::runtime_error(err_info);
	}
	init_success.glfw = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Apple hates OpenGL, literally
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
				   GL_TRUE);  // Uncomment this statement if on Apple's macOS
#endif

	// Verbose monitor info
	const auto* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	logger.log(Logger::Info,
			   "Display: {}x{}, {}Hz, R{}G{}B{}",
			   video_mode->width,
			   video_mode->height,
			   video_mode->refreshRate,
			   video_mode->redBits,
			   video_mode->greenBits,
			   video_mode->blueBits);

	// Initialize Window
	window = glfwCreateWindow(std::max(800, video_mode->width / 2),
							  std::max(600, video_mode->height / 2),
							  "Mandelbrot Viewer",
							  nullptr,
							  nullptr);

	if (window == nullptr)
	{
		const char* err_info;
		glfwGetError(&err_info);

		throw std::runtime_error(err_info);
	}
	init_success.window = true;

	// Configure window
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		auto		err_code = glGetError();
		const char* err_info = (const char*)glewGetErrorString(err_code);

		throw std::runtime_error(err_info);
	}
	init_success.glew = true;
}

App::~App()
{
	delete logic;
	if (init_success.imgui)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	if (init_success.window) glfwDestroyWindow(window);
	if (init_success.glfw) glfwTerminate();
}

void App::init_imgui()
{
	init_success.imgui = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetCurrentContext();

	// Setup Platform/Renderer bindings
	init_success.imgui &= ImGui_ImplGlfw_InitForOpenGL(window, true);
	init_success.imgui &= ImGui_ImplOpenGL3_Init(gl_version);

	if (!init_success.imgui)
	{
		throw std::runtime_error("Initialize IMGUI Failed");
	}

	// Setup style
	ImGui::StyleColorsDark();

	float gui_scale = util::get_gui_scale(window);

	auto* style = &ImGui::GetStyle();
	{  // configure detailed style
		style->WindowRounding = 4;
		style->FrameRounding  = 4;
		style->GrabRounding	  = 4;
		style->PopupRounding  = 4;
		style->ChildRounding  = 4;
	}
	style->ScaleAllSizes(gui_scale);

	ImFontConfig font_config;
	std::snprintf(font_config.Name, sizeof(font_config.Name), "%s", "HarmonyOs Sans Regular");
	font_config.FontDataOwnedByAtlas = false;  // Prevent owner transferring

	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
		(void*)resources::file_HarmonyOS_Sans_Regular_ttf_.data(),
		(int)resources::file_HarmonyOS_Sans_Regular_ttf_.size(),
		16.0f * gui_scale,
		&font_config);
}

void App::run()
{
	if (!init_success.success())
	{
		logger.log(Logger::Warning, "Failed to initialize, stopping");
		return;
	}

	glClearColor(0, 0, 0, 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		auto [width, height] = util::get_framebuffer_size(window);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		logic->update(width, height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}