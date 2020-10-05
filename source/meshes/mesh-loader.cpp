#include "meshes/mesh-loader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>

std::unique_ptr<TriangleMesh> meshloader::loadObj(const char* location) {
	std::ifstream file(location, std::ios::in);
	std::vector<Point3> positions;
	std::vector<Point2> uvs;
	std::vector<Vector3> normals;
	std::vector<std::shared_ptr<Vertex>> vertices;
	std::vector<int> indices;
	std::map<std::tuple<int, int, int>, int> ptn;
	std::string in;

	if (!file) {
		std::cout << "Could not load: " << location << std::endl;
		std::exit(1);
	}

	while (file) {
		file >> in;
		if (in == "v") {
			float x, y, z;
			file >> x;
			file >> y;
			file >> z;
			positions.push_back(Point3(x, y, z));
		}

		else if (in == "vt") {
			float x, y;
			file >> x;
			file >> y;
			uvs.push_back(Point2(x, y));
		}

		else if (in == "vn") {
			float x, y, z;
			file >> x;
			file >> y;
			file >> z;
			normals.push_back(glm::normalize(Vector3(x, y, z)));
		}

		else if (in == "f") {
			for (int i = 0; i < 3; ++i) {
				int p, t, n;
				file >> p;
				file.ignore();
				file >> t;
				file.ignore();
				file >> n;
				std::tuple<int, int, int> v = std::make_tuple(p - 1, t - 1, n - 1);

				if (ptn.find(v) == ptn.end()) {
					vertices.push_back(
						std::make_shared<Vertex>(positions[p - 1], uvs[t - 1], normals[n - 1]));
					indices.push_back(indices.size());
				}

				else {
					ptn[v] = indices.size();
					indices.push_back(ptn[v]);
				}
			}
		}
	}
	return std::make_unique<TriangleMesh>(vertices, indices);
}
