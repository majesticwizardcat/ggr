NAME = ggr

INCLUDES = -Isource -Ilibraries
SOURCE = source/accelerators/*.cpp source/shaders/*.cpp source/core/*.cpp source/film/*.cpp source/cameras/*.cpp source/filters/*.cpp source/samplers/*.cpp source/images/*.cpp source/primitives/*.cpp source/entities/*.cpp source/lighting/*.cpp source/materials/*.cpp source/spectra/*.cpp source/tools/*.cpp source/intersection/*.cpp source/textures/*.cpp source/meshes/*.cpp source/integrators/*.cpp

OPTIONS = -pthread -O3 -std=c++17

default: clean
	g++ ${INCLUDES} ${OPTIONS} ${SOURCE} source/ggr.cpp -o ggr

clean:
	rm -f ggr

cleanexample0:
	rm -f example0

example0: cleanexample0
	g++ ${INCLUDES} ${OPTIONS} ${SOURCE} examples/example-0.cpp -o example0

