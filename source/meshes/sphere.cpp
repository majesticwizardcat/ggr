#include "meshes/sphere.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

#include <cmath>

Sphere::Sphere() : Sphere(DEFAULT_CIRCLES) { }

Sphere::Sphere(size_t circles) {
	for (size_t i = 0; i < circles; ++i) {
		float theta = PI - ((i + 1) * (PI / (float) (circles + 1)));
		Point3 pos;
		Point2 uv;
		Vector3 n;
		for (size_t j = 0; j < circles; ++j) {
			float phi = (TWO_PI / (float) circles) * j;
			pos = shading::fromSpherical(phi, theta);
			uv = Point2(phi / TWO_PI, 1.0f - (0.5f + pos.z / 2.0f));
			n = pos;
			m_vertices.push_back(Vertex(pos, uv, n));
		}
		pos = shading::fromSpherical(0.0f, theta);
		uv = Point2(1.0f, 1.0f - (0.5f + pos.z / 2.0f));
		n = pos;
		m_vertices.push_back(Vertex(pos, uv, n));
	}

	for (size_t i = 0; i < circles - 1; ++i) {
		size_t startIndex = i * circles + i;
		for (size_t j = 0; j < circles; ++j) {
			size_t i0 = startIndex + j;
			size_t i1 = startIndex + j + 1;
			size_t i2 = startIndex + circles + 1 + j;
			size_t i3 = startIndex + circles + 1 + j + 1;
			m_triangles.push_back(std::make_unique<Triangle>(
				&m_vertices[i0], &m_vertices[i1], &m_vertices[i2]));
			m_triangles.push_back(std::make_unique<Triangle>(
				&m_vertices[i1], &m_vertices[i3], &m_vertices[i2]));

		}
	}

	size_t startIndex = (circles - 1) * (circles + 1);
	for (size_t i = 0; i < circles; ++i) {
		size_t botIndex = m_vertices.size();
		size_t topIndex = botIndex + 1;
		float phi = (TWO_PI / (float) circles) * i;
		float phiN = (TWO_PI / (float) circles) * (i + 1);
		float u = (phi / TWO_PI + phiN / TWO_PI) / 2.0f;
		m_vertices.push_back(Vertex(
					Point3(0.0f, 0.0f, -1.0f),
					Point2(u, 1.0f), 
					Vector3(0.0f, 0.0f, -1.0f))
				);
		m_vertices.push_back(Vertex(
					Point3(0.0f, 0.0f, 1.0f),
					Point2(u, 0.0f),
					Vector3(0.0f, 0.0f, 1.0f))
				);
		m_triangles.push_back(std::make_unique<Triangle>(
			&m_vertices[botIndex], &m_vertices[i + 1], &m_vertices[i]));
		m_triangles.push_back(std::make_unique<Triangle>(
			&m_vertices[topIndex], &m_vertices[startIndex + i],
			&m_vertices[startIndex + i + 1]));
	}
}
