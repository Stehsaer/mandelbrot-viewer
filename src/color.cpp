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

#include "color.hpp"
#include <algorithm>

/* NOTE: These color conversion codes come from
 * https://blog.csdn.net/lz0499/article/details/77345166
 * Slightly modified.
 */

static float gamma_rgb2lab(float x)
{
	return x > 0.04045 ? powf((x + 0.055f) / 1.055f, 2.4f) : (x / 12.92f);
};

static float gamma_xyz2rgb(float x)
{
	return x > 0.0031308 ? (1.055f * powf(x, (1 / 2.4f)) - 0.055f) : (x * 12.92f);
}

static const float param_13	   = 1.0f / 3.0f;
static const float param_16116 = 16.0f / 116.0f;
static const float x_n		   = 0.950456f;
static const float y_n		   = 1.0f;
static const float z_n		   = 1.088754f;

LAB_color SRGB_color::to_lab() const
{
	return to_xyz().to_lab();
}

SRGB_color LAB_color::to_srgb() const
{
	return to_xyz().to_srgb();
}

XYZ_color SRGB_color::to_xyz() const
{
	float r_gamma = gamma_rgb2lab(r);
	float g_gamma = gamma_rgb2lab(g);
	float b_gamma = gamma_rgb2lab(b);

	float x = 0.4124564f * r_gamma + 0.3575761f * g_gamma + 0.1804375f * b_gamma;
	float y = 0.2126729f * r_gamma + 0.7151522f * g_gamma + 0.0721750f * b_gamma;
	float z = 0.0193339f * r_gamma + 0.1191920f * g_gamma + 0.9503041f * b_gamma;

	return {x, y, z};
}

LAB_color XYZ_color::to_lab() const
{
	float f_x, f_y, f_z;

	float x = this->x / x_n;
	float y = this->y / y_n;
	float z = this->z / z_n;

	if (y > 0.008856f)
		f_y = pow(y, param_13);
	else
		f_y = 7.787f * y + param_16116;

	if (x > 0.008856f)
		f_x = pow(x, param_13);
	else
		f_x = 7.787f * x + param_16116;

	if (z > 0.008856)
		f_z = pow(z, param_13);
	else
		f_z = 7.787f * z + param_16116;

	float l = 116.0f * f_y - 16.0f;
	l		= l > 0.0f ? l : 0.0f;
	float a = 500.0f * (f_x - f_y);
	float b = 200.0f * (f_y - f_z);

	return {l, a, b};
}

XYZ_color LAB_color::to_xyz() const
{
	float f_x, f_y, f_z, x, y, z;

	f_y = (l + 16.0f) / 116.0f;
	f_x = a / 500.0f + f_y;
	f_z = f_y - b / 200.0f;

	if (powf(f_y, 3.0) > 0.008856)
		y = powf(f_y, 3.0);
	else
		y = (f_y - param_16116) / 7.787f;

	if (powf(f_x, 3) > 0.008856)
		x = f_x * f_x * f_x;
	else
		x = (f_x - param_16116) / 7.787f;

	if (powf(f_z, 3.0) > 0.008856)
		z = f_z * f_z * f_z;
	else
		z = (f_z - param_16116) / 7.787f;

	x *= x_n;
	y *= y_n;
	z *= z_n;

	return {x, y, z};
}

SRGB_color XYZ_color::to_srgb() const
{
	float r, g, b;
	r = 3.2404542f * x - 1.5371385f * y - 0.4985314f * z;
	g = -0.9692660f * x + 1.8760108f * y + 0.0415560f * z;
	b = 0.0556434f * x - 0.2040259f * y + 1.0572252f * z;

	r = gamma_xyz2rgb(r);
	g = gamma_xyz2rgb(g);
	b = gamma_xyz2rgb(b);

	return {r, g, b};
}

SRGB_color SRGB_color::lerp(const SRGB_color& min, const SRGB_color& max, float lerp)
{
	return LAB_color::lerp(min.to_lab(), max.to_lab(), lerp).to_srgb();
}

LAB_color LAB_color::lerp(const LAB_color& min, const LAB_color& max, float lerp)
{
	return {std::lerp(min.l, max.l, lerp),
			std::lerp(min.a, max.a, lerp),
			std::lerp(min.b, max.b, lerp)};
}

std::vector<uint8_t> SRGB_color::to_byte(const std::vector<SRGB_color>& colors)
{
	std::vector<uint8_t> output;
	output.resize(colors.size() * 3);

	for (size_t i = 0; i < colors.size(); i++)
	{
		output[i * 3]	  = (int)round(std::clamp(colors[i].r * 255.0f, 0.0f, 255.0f));
		output[i * 3 + 1] = (int)round(std::clamp(colors[i].g * 255.0f, 0.0f, 255.0f));
		output[i * 3 + 2] = (int)round(std::clamp(colors[i].b * 255.0f, 0.0f, 255.0f));
	}

	return output;
}