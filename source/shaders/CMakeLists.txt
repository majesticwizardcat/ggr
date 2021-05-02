add_library(Shaders mf-reflection-shader.cpp
		mf-refraction-shader.cpp
		refraction-shader.cpp)

target_link_libraries(Shaders Textures)
