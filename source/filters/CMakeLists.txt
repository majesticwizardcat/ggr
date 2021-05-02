add_library(Filters box-filter.cpp
		gaussian-filter.cpp
		lanczos-filter.cpp)

target_link_libraries(Filters Images)

