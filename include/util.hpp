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

/*
DESCRIPTION:
Provides Result and Logger class
*/

#pragma once

#include "common-include.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

// Result class representing either a value or an error
template <typename Val_T, typename Err_T> struct Result
{
  private:
	std::variant<Val_T, Err_T> value;

  public:
	[[nodiscard]] bool ok() const { return std::holds_alternative<Val_T>(value); }

	Result(Val_T&& val) :
		value(std::move(val))
	{}

	Result(Err_T&& err) :
		value(std::move(err))
	{}

	Val_T&& get() { return std::forward<Val_T>(std::get<Val_T>(value)); }
	Err_T&& get_err() { return std::forward<Err_T>(std::get<Err_T>(value)); }
};

// Logger class for logging to console and/or file
class Logger
{
  public:
	enum Output_target
	{
		Console = 0x01,
		File	= 0x02
	};

	enum Log_level
	{
		Info,
		Warning,
		Error
	};

	Output_target target;

	Logger(Output_target target) :
		target(target)
	{
		log(Info, "[====== Log Start ======]");
	}

	~Logger()
	{
		log(Info, "[====== Log End ======]");

		// force close the file stream
		close();
	}

	// Close the file stream
	void close()
	{
		if (file_stream.is_open()) file_stream.close();
	}

	// Open a file for logging, returns true if successful
	bool open(const std::string& path, bool append = false)
	{
		file_stream.open(path,
						 append ? std::ios_base::app | std::ios_base::out : std::ios_base::out);
		return file_stream.is_open();
	}

	template <typename... Arg_T>
	void log(Log_level level, const std::format_string<Arg_T...> fmt, Arg_T&&... args)
	{
		auto formatted_string = std::format(fmt, std::forward<Arg_T>(args)...);
		log(level, formatted_string);
	}

	// Log a string
	void log(Log_level level, const std::string& str)
	{
		if (target & Console)
			std::cout << get_current_time_str() << "  " << log_level_color_start(level)
					  << log_level_string(level) << str << log_level_color_end()
					  << '\n';	// To console

		if (target & File && file_stream.is_open())
			file_stream << get_current_time_str() << "  " << log_level_string(level) << str
						<< '\n';  // To file
	}

  private:
	std::ofstream file_stream;

	std::chrono::time_point<std::chrono::steady_clock> start_time
		= std::chrono::steady_clock::now();

	static const char* log_level_string(Log_level level)
	{
		switch (level)
		{
		case Info:
			return "[INFO]> ";
		case Warning:
			return "[WARNING]> ";
		case Error:
			return "[ERROR]> ";
		}

		return "[UNKNOWN]>";
	}

	static const char* log_level_color_start(Log_level level)
	{
		switch (level)
		{
		case Info:
			return "\033[0;37m";
		case Warning:
			return "\033[1;33m";
		case Error:
			return "\033[1;31m";
		}

		return "\033[0m";
	}

	static const char* log_level_color_end() { return "\033[0m"; }

	std::string get_current_time_str()
	{
		auto now	  = std::chrono::steady_clock::now();
		auto duration = now - start_time;

		auto seconds
			= (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
			/ 1000.0;

		return std::format("{:.3f}", seconds);
	}
};

// Global Logger
inline Logger logger{Logger::Console};

namespace util
{
// Get GLFW window content scale
inline float get_gui_scale(GLFWwindow* wnd)
{
	float x, y;
	glfwGetWindowContentScale(wnd, &x, &y);
	return std::max(x, y);
}

// Get GLFW window framebuffer size
inline std::tuple<int, int> get_framebuffer_size(GLFWwindow* wnd)
{
	int x, y;
	glfwGetFramebufferSize(wnd, &x, &y);
	return {x, y};
}

inline void check_err(const char* where)
{
	while (true)
	{
		auto err_code = glGetError();
		if (err_code != GL_NO_ERROR)
		{
			logger.log(Logger::Error, "At {}: Opengl Error {}", where, err_code);
		}
		else
			break;
	}
}
}  // namespace util