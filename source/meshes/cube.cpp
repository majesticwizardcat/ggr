#include "meshes/cube.h"

Cube::Cube() : TriangleMesh() {
	Point3 p1( 0.5f,  0.5f, -0.5f);
	Point3 p2( 0.5f, -0.5f, -0.5f);
	Point3 p3( 0.5f,  0.5f,  0.5f);
	Point3 p4( 0.5f, -0.5f,  0.5f);
	Point3 p5(-0.5f,  0.5f, -0.5f);
	Point3 p6(-0.5f, -0.5f, -0.5f);
	Point3 p7(-0.5f,  0.5f,  0.5f);
	Point3 p8(-0.5f, -0.5f,  0.5f);

	Point2 uv1( 0.50f, 0.0f);
	Point2 uv2( 0.25f, 0.25f);
	Point2 uv3( 0.25f, 0.0f);
	Point2 uv4( 0.50f, 0.25f);
	Point2 uv5( 0.25f, 0.5f);
	Point2 uv6( 0.25f, 0.25f);
	Point2 uv7( 0.50f, 0.5f);
	Point2 uv8( 0.25f, 0.75f);
	Point2 uv9( 0.50f, 0.75f);
	Point2 uv10(0.25f, 1.0f);
	Point2 uv11(0.25f, 0.5f);
	Point2 uv12(0.0f,  0.75f);
	Point2 uv13(0.0f,  0.5f);
	Point2 uv14(0.75f, 0.5f);
	Point2 uv15(0.50f, 0.5f);
	Point2 uv16(0.50f, 0.25f);
	Point2 uv17(0.50f, 0.75f);
	Point2 uv18(0.50f, 1.0f);
	Point2 uv19(0.25f, 0.75f);
	Point2 uv20(0.75f, 0.75f);

	Normal n1( 0.0f,  1.0f,  0.0f);
	Normal n2( 0.0f,  0.0f,  1.0f);
	Normal n3(-1.0f,  0.0f,  0.0f);
	Normal n4( 0.0f, -1.0f,  0.0f);
	Normal n5( 1.0f,  0.0f,  0.0f);
	Normal n6( 0.0f,  0.0f, -1.0f);

	m_vertices.push_back(std::make_shared<Vertex>(p5, uv1, n1));
	m_vertices.push_back(std::make_shared<Vertex>(p3, uv2, n1));
	m_vertices.push_back(std::make_shared<Vertex>(p1, uv3, n1));
	
	m_vertices.push_back(std::make_shared<Vertex>(p3, uv4, n2));
	m_vertices.push_back(std::make_shared<Vertex>(p8, uv5, n2));
	m_vertices.push_back(std::make_shared<Vertex>(p4, uv6, n2));

	m_vertices.push_back(std::make_shared<Vertex>(p7, uv7, n3));
	m_vertices.push_back(std::make_shared<Vertex>(p6, uv8, n3));
	m_vertices.push_back(std::make_shared<Vertex>(p8, uv5, n3));

	m_vertices.push_back(std::make_shared<Vertex>(p2, uv9, n4));
	m_vertices.push_back(std::make_shared<Vertex>(p8, uv10, n4));
	m_vertices.push_back(std::make_shared<Vertex>(p6, uv8, n4));

	m_vertices.push_back(std::make_shared<Vertex>(p1, uv11, n5));
	m_vertices.push_back(std::make_shared<Vertex>(p4, uv12, n5));
	m_vertices.push_back(std::make_shared<Vertex>(p2, uv13, n5));

	m_vertices.push_back(std::make_shared<Vertex>(p5, uv14, n6));
	m_vertices.push_back(std::make_shared<Vertex>(p2, uv9, n6));
	m_vertices.push_back(std::make_shared<Vertex>(p6, uv15, n6));

	m_vertices.push_back(std::make_shared<Vertex>(p7, uv16, n1));

	m_vertices.push_back(std::make_shared<Vertex>(p7, uv7, n2));

	m_vertices.push_back(std::make_shared<Vertex>(p5, uv17, n3));

	m_vertices.push_back(std::make_shared<Vertex>(p4, uv18, n4));

	m_vertices.push_back(std::make_shared<Vertex>(p3, uv19, n5));

	m_vertices.push_back(std::make_shared<Vertex>(p1, uv20, n6));

	int indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
			10, 11, 12, 13, 14, 15, 16, 17,
			0, 18, 1, 3, 19, 4, 6, 20, 7,
			9, 21, 10, 12, 22, 13, 15, 23, 16 };

	for (int i = 0; i < 36; i += 3) {
		m_triangles.push_back(std::make_shared<Triangle>(m_vertices[indices[i]],
					m_vertices[indices[i + 1]],
					m_vertices[indices[i + 2]]));
	}
}

Cube::Cube(const Cube& other) : TriangleMesh(other) { }

