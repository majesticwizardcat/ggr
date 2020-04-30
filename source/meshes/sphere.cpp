#include "meshes/sphere.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

#include <cmath>

Sphere::Sphere() : Sphere(DEFAULT_CIRCLES) { }

Sphere::Sphere(int circles) {
	for (int i = 0; i < circles; ++i) {
		float theta = PI - ((i + 1) * (PI / (float) (circles + 1)));
		Point3 pos;
		Point2 uv;
		Normal n;
		for (int j = 0; j < circles; ++j) {
			float phi = (TWO_PI / (float) circles) * j;
			pos = shading::fromSpherical(phi, theta);
			uv = Point2(phi / TWO_PI, 1.0f - (0.5f + pos.z / 2.0f));
			n = Normal(pos);
			m_vertices.push_back(std::make_shared<Vertex>(pos, uv, n));
		}
		pos = shading::fromSpherical(0.0f, theta);
		uv = Point2(1.0f, 1.0f - (0.5f + pos.z / 2.0f));
		n = Normal(pos);
		m_vertices.push_back(std::make_shared<Vertex>(pos, uv, n));
	}

	for (int i = 0; i < circles - 1; ++i) {
		int startIndex = i * circles + i;
		for (int j = 0; j < circles; ++j) {
			int i0 = startIndex + j;
			int i1 = startIndex + j + 1;
			int i2 = startIndex + circles + 1 + j;
			int i3 = startIndex + circles + 1 + j + 1;
			m_triangles.push_back(std::make_shared<Triangle>(
				m_vertices[i0], m_vertices[i1], m_vertices[i2]));
			m_triangles.push_back(std::make_shared<Triangle>(
				m_vertices[i1], m_vertices[i3], m_vertices[i2]));

		}
	}

	int startIndex = (circles - 1) * (circles + 1);
	for (int i = 0; i < circles; ++i) {
		int botIndex = m_vertices.size();
		int topIndex = botIndex + 1;
		float phi = (TWO_PI / (float) circles) * i;
		float phiN = (TWO_PI / (float) circles) * (i + 1);
		float u = (phi / TWO_PI + phiN / TWO_PI) / 2.0f;
		m_vertices.push_back(std::make_shared<Vertex>(Point3(0.0f, 0.0f, -1.0f),
									Point2(u, 1.0f),
									Normal(0.0f, 0.0f, -1.0f)));
		m_vertices.push_back(std::make_shared<Vertex>(Point3(0.0f, 0.0f, 1.0f),
									Point2(u, 0.0f),
									Normal(0.0f, 0.0f, 1.0f)));
		m_triangles.push_back(std::make_shared<Triangle>(
			m_vertices[botIndex], m_vertices[i + 1], m_vertices[i]));
		m_triangles.push_back(std::make_shared<Triangle>(
			m_vertices[topIndex], m_vertices[startIndex + i],
			m_vertices[startIndex + i + 1]));
	}
}

Sphere::Sphere(const Sphere& other) : TriangleMesh(other) { }

