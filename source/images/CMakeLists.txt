add_library(Images checkerboard-image.cpp
	image-io.cpp
	image.cpp
	mipmap.cpp)

target_link_libraries(Images Cameras)

