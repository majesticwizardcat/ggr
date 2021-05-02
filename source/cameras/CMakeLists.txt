add_library(Cameras camera.cpp
	orthographic-camera.cpp
	perspective-camera.cpp
	projective-camera.cpp)

target_link_libraries(Cameras Tools)

