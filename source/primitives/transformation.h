#pragma once

class Transformation;

#include "primitives/matrix.h"
#include "primitives/vector.h"
#include "primitives/point.h"
#include "intersection/ray.h"
#include "meshes/vertex.h"
#include "tools/util.h"

class Transformation {
private:
	Matrix4 m_transform;
	Matrix4 m_inverse;

public:
	Transformation() : Transformation(Matrix4(1.0f), Matrix4(1.0f)) { } 
	Transformation(const Transformation& other) : Transformation(other.m_transform, other.m_inverse) { }
	Transformation(const Matrix4& transform) : Transformation(transform, glm::inverse(transform)) { }
	Transformation(const Matrix4& transform, const Matrix4& inverse) : 
		m_transform(transform), m_inverse(inverse) { }

	Vertex applyVertex(const Vertex& v) const;
	Vertex applyInverseVertex(const Vertex& v) const;

	void applyRay(Ray* r) const;
	void applyInverseRay(Ray* r) const;

	inline bool operator==(const Transformation& other) const {
		return m_transform == other.m_transform;
	}

	inline Transformation transpose() const {
		return Transformation(
				glm::transpose(m_transform), 
				glm::transpose(m_inverse));
	}

	inline Transformation inverse() const {
		return Transformation(m_inverse, m_transform);
	}

	inline Transformation combine(const Transformation& right) const {
		return Transformation(
				m_transform * right.m_transform, 
				right.m_inverse * m_inverse);
	}

	inline Transformation operator*(const Transformation& right) const {
		return combine(right);
	}

	inline Vector4 applyVector(const Vector4& v) const {
		return m_transform * v;
	}

	inline Vector3 applyVector(const Vector3& v) const {
		return m_transform * Vector4(v, 0.0f);
	}

	inline Vector2 applyVector(const Vector2& v) const {
		return m_transform * Vector4(v, 0.0f, 0.0f);
	}

	inline Vector3 applyNormal(const Vector3& n) const {
		return glm::transpose(m_inverse) * Vector4(n, 0.0f);
	}

	inline Point4 applyPoint(const Point4& p) const {
		Point4 res = m_transform * p;

		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}

	inline Point3 applyPoint(const Point3& p) const {
		Point4 res = m_transform * Point4(p, 1.0f);
		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}

	inline Point2 applyPoint(const Point2& p) const {
		Point4 res = m_transform * Point4(p, 0.0f, 1.0f);
		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}

	inline Vector4 applyInverseVector(const Vector4& v) const {
		return m_inverse * v;
	}

	inline Vector3 applyInverseVector(const Vector3& v) const {
		return m_inverse * Vector4(v, 0.0f);
	}

	inline Vector2 applyInverseVector(const Vector2& v) const {
		return m_inverse * Vector4(v, 0.0f, 0.0f);
	}

	inline Vector3 applyInverseNormal(const Vector3& n) const {
		return glm::transpose(m_transform) * Vector4(n, 0.0f);
	}

	inline Point4 applyInversePoint(const Point4& p) const {
		Point4 res = m_inverse * p;

		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}

	inline Point3 applyInversePoint(const Point3& p) const {
		Point4 res = m_inverse * Point4(p, 1.0f);

		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}

	inline Point2 applyInversePoint(const Point2& p) const {
		Point4 res = m_inverse * Point4(p, 0.0f, 1.0f);

		if (res.w != 0.0f) {
			res = res / res.w;
		}

		return res;
	}
};

namespace transform {
Transformation translate(float x, float y, float z);
Transformation scale(float sx, float sy, float sz);
Transformation rotateX(float radians);
Transformation rotateY(float radians);
Transformation rotateZ(float radians);
Transformation rotate(float rx, float ry, float rz);
Transformation rotateAxis(float radians, const Vector3& axis);
Transformation transform(float x, float y, float z,
			 float sx, float sy, float sz,
			 float rx, float ry, float rz);
Transformation lookAt(const Point3& position, const Point3& look, const Vector3& up);
Transformation view(const Point3& position, const Point3& look, const Vector3& up);
Transformation perspectiveProjection(float fov, float aspectRatio, float near, float far);
Transformation orthographicProjection(float startX, float startY, float endX, float endY);
}
