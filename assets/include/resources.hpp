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
#include <string>
#include <vector>

namespace resources
{
using Binary_resource = std::vector<unsigned char>;

extern const Binary_resource file_HarmonyOS_Sans_Regular_ttf_, file_shaders_generator_frag_,
	file_shaders_common_vert_, file_shaders_shader_test_frag_;

inline std::string to_string(const Binary_resource& resource)
{
	return std::string((char*)resource.data(), resource.size());
}

}  // namespace resources