cmake_minimum_required(VERSION 3.20)
project("Mandelbrot Viewer")

# Packages
find_package(glfw3 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(imgui)

add_library(app STATIC)

aux_source_directory(src/ app_src)
aux_source_directory(include/ app_hpp)
target_sources(app PRIVATE ${app_src} ${app_hpp})
target_include_directories(app PUBLIC include/)

target_compile_features(app PUBLIC cxx_std_20)

# link & include
target_include_directories(app PUBLIC ${Stb_INCLUDE_DIR})
target_link_libraries(app PUBLIC glfw)
target_link_libraries(app PUBLIC GLEW::GLEW)
target_link_libraries(app PUBLIC OpenGL::GL)
target_link_libraries(app PUBLIC imgui)
target_link_libraries(app PUBLIC glm::glm)

# Embed resources
add_library(resources STATIC)
target_include_directories(resources PUBLIC assets/include)
target_compile_features(resources PUBLIC cxx_std_20)

# Check if the directory exists and create it if necessary
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/embed")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/embed/shaders")

set(resource_files HarmonyOS_Sans_Regular.ttf shaders/generator.frag shaders/common.vert shaders/shader-test.frag)

foreach(file ${resource_files})
	string(REGEX REPLACE "[/.\\\\-]" "_" file_name ${file})
	message(STATUS "Find resource: ${file}, name: file_${file_name}_")
	add_custom_command(
		OUTPUT "${CMAKE_BINARY_DIR}/embed/${file}.cpp"
		COMMAND python ${CMAKE_SOURCE_DIR}/embed.py "${CMAKE_SOURCE_DIR}/assets/${file}" "${CMAKE_BINARY_DIR}/embed/${file}.cpp" "file_${file_name}_"
		COMMENT "Embedding ${file}"
		DEPENDS "assets/${file}"
	)
	target_sources(resources PRIVATE "${CMAKE_BINARY_DIR}/embed/${file}.cpp")
endforeach()

target_link_libraries(app PUBLIC resources)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE app)

add_subdirectory(test)