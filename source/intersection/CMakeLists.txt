add_library(Intersection intersection.cpp
		ray.cpp)

target_link_libraries(Intersection Meshes)
