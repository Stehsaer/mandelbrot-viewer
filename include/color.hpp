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

struct SRGB_color;
struct LAB_color;
struct XYZ_color;

struct SRGB_color
{
	float r, g, b;

	[[nodiscard]] LAB_color to_lab() const;
	[[nodiscard]] XYZ_color to_xyz() const;

	static SRGB_color lerp(const SRGB_color& min, const SRGB_color& max, float lerp);

	static std::vector<uint8_t> to_byte(const std::vector<SRGB_color>& colors);
};

struct LAB_color
{
	float l, a, b;

	[[nodiscard]] SRGB_color to_srgb() const;
	[[nodiscard]] XYZ_color	 to_xyz() const;

	static LAB_color lerp(const LAB_color& min, const LAB_color& max, float lerp);
};

struct XYZ_color
{
	float x, y, z;

	[[nodiscard]] LAB_color	 to_lab() const;
	[[nodiscard]] SRGB_color to_srgb() const;
};