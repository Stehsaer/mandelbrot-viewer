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

#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

// #define GLEW_STATIC
#include <gl/glew.h>  // GLEW must be included before GLFW

#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
