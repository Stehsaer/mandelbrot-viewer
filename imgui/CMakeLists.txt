

add_library(imgui STATIC)
aux_source_directory(src imgui_src)
target_sources(imgui PRIVATE ${imgui_src})
target_include_directories(imgui PUBLIC include)

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE GLEW::GLEW)
target_link_libraries(imgui PRIVATE Freetype::Freetype)

target_compile_features(imgui PUBLIC cxx_std_20)