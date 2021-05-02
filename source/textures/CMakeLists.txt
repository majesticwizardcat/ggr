add_library(Textures color-texture.cpp
	height-map.cpp
	image-texture.cpp)

target_link_libraries(Textures Intersection)

