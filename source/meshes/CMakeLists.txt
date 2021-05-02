add_library(Meshes cube.cpp
		mesh-loader.cpp
		plane.cpp
		sphere.cpp
		triangle-mesh.cpp
		triangle.cpp)

target_link_libraries(Meshes Film)
