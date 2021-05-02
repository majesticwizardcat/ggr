add_library(Samplers halton-sampler.cpp
		sampler.cpp
		stohastic-sampler.cpp)
target_link_libraries(Samplers Filters)

