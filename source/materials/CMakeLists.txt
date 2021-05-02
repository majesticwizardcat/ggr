add_library(Materials blend-material.cpp
		glass-material.cpp
		metal-material.cpp
		plastic-material.cpp)

target_link_libraries(Materials Lighting)
