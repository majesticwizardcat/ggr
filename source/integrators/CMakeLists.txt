add_library(Integrators film-sampling-integrator.cpp
		integrator.cpp
		path-integrator.cpp)
target_link_libraries(Integrators Accelerators)
