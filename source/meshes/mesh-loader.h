#pragma once

#include "meshes/triangle-mesh.h"

#include <memory>

namespace meshloader {
	std::unique_ptr<TriangleMesh> loadObj(const char* location);
}
