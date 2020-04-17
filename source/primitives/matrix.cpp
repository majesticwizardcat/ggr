#include "primitives/matrix.h"
#include "tools/util.h"

#include <iostream>
#include <string.h>
#include <math.h>

Matrix2::Matrix2() {
	memset(m_data, 0, sizeof(m_data));
}

Matrix2::Matrix2(const Matrix2& other) : Matrix2(other.m_data) { }

Matrix2::Matrix2(const float data[]) {
	for (int i = 0; i < 4; ++i) {
		m_data[i] = data[i];
	}	
}

Matrix2::Matrix2(float diagonal) : Matrix2() {
	set(0, 0, diagonal);
	set(1, 1, diagonal);
}

Matrix2::Matrix2(float m00, float m10,
		float m01, float m11) {
	set(0, 0, m00);
	set(1, 0, m10);
	set(0, 1, m01);
	set(1, 1, m11);
}

void Matrix2::set(int col, int row, float value) {
	m_data[col * 2 + row] = value;
}

void Matrix2::print() const {
	for (int r = 0; r < 2; ++r) {
		for (int c = 0; c < 2; ++c) {
			std::cout << get(c, r) << " ";
		}
		std::cout << std::endl;
	}
}

float Matrix2::det() const {
	return get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0);
}

float Matrix2::trace() const {
	return get(0, 0) + get(1, 1);
}

float Matrix2::get(int col, int row) const {
	return m_data[col * 2 + row];
}

bool Matrix2::isInvertible() const {
	return !util::equals(det(), 0.0f);
}

bool Matrix2::operator==(const Matrix2& right) const {
	for (int i = 0; i < 4; ++i) {
		if (!util::equals(m_data[i], right.m_data[i])) {
			return false;
		}
	}
	return true;
}

Matrix2 Matrix2::multiply(const Matrix2& right) const {
	Matrix2 result;
	float value;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			value = 0.0f;
			for (int k = 0; k < 2; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}
	return result;
}

Matrix2 Matrix2::inverse() const {
	float dt = det();
	
	if (util::equals(dt, 0.0f)) {
		return Matrix2(1.0f);
	}

	float oneOverDet = 1.0f / dt;
	return (Matrix2(trace()) - *this) * oneOverDet;
}

Matrix2 Matrix2::transpose() const {
	Matrix2 trs;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			trs.set(i, j, get(j, i));
		}
	}

	return trs;
}

Matrix2 Matrix2::operator+(const Matrix2& right) const {
	Matrix2 result;
	for (int i = 0; i < 4; ++i) {
		result.m_data[i] = m_data[i] + right.m_data[i];
	}
	return result;
}

Matrix2 Matrix2::operator-(const Matrix2& right) const {
	Matrix2 result;
	for (int i = 0; i < 4; ++i) {
		result.m_data[i] = m_data[i] - right.m_data[i];
	}
	return result;
}

Matrix2 Matrix2::operator*(const Matrix2& right) const {
	return multiply(right);
}

Matrix2 Matrix2::operator*(float scalar) const {
	Matrix2 result;
	for (int i = 0; i < 4; ++i) {
		result.m_data[i] = m_data[i] * scalar;
	}
	return result;
}

Matrix2& Matrix2::operator+=(const Matrix2& right) {
	for (int i = 0; i < 4; ++i) {
		m_data[i] = m_data[i] + right.m_data[i];
	}
	return *this;
}

Matrix2& Matrix2::operator-=(const Matrix2& right) {
	for (int i = 0; i < 4; ++i) {
		m_data[i] = m_data[i] - right.m_data[i];
	}
	return *this;
}

Matrix2& Matrix2::operator*=(const Matrix2& right) {
	Matrix2 result;
	float value;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			value = 0.0f;
			for (int k = 0; k < 2; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}

	for (int i = 0; i < 4; ++i) {
		m_data[i] = result.m_data[i];
	}
	return *this;
}

Matrix2& Matrix2::operator*=(float scalar) {
	for (int i = 0; i < 4; ++i) {
		m_data[i] *= scalar;
	}
	return *this;
}

Vector2 Matrix2::multiply(const Vector2& right) const {
	Vector2 result;
	result.x = get(0, 0) * right.x + get(1, 0) * right.y;
	result.y = get(0, 1) * right.x + get(1, 1) * right.y;
	return result;
}

Vector2 Matrix2::operator*(const Vector2& right) const {
	return multiply(right);
}

Point2 Matrix2::multiply(const Point2& right) const {
	Point2 result;
	result.x = get(0, 0) * right.x + get(1, 0) * right.y;
	result.y = get(0, 1) * right.x + get(1, 1) * right.y;
	return result;
}

Point2 Matrix2::operator*(const Point2& right) const {
	return multiply(right);
}

Matrix3::Matrix3() {
	memset(m_data, 0, sizeof(m_data));
}

Matrix3::Matrix3(const Matrix3& other) : Matrix3(other.m_data) { }

Matrix3::Matrix3(const float data[]) {
	for (int i = 0; i < 9; ++i) {
		m_data[i] = data[i];
	}
}

Matrix3::Matrix3(float diagonal) : Matrix3() {
	for (int i = 0; i < 3; ++i) {
		set(i, i, diagonal);
	}
}

Matrix3::Matrix3(float m00, float m10, float m20,
		float m01, float m11, float m21,
		float m02, float m12, float m22) {
	set(0, 0, m00);
	set(0, 1, m01);
	set(0, 2, m02);
	set(1, 0, m10);
	set(1, 1, m11);
	set(1, 2, m12);
	set(2, 0, m20);
	set(2, 1, m21);
	set(2, 2, m22);
}

void Matrix3::print() const {
	for (int r = 0; r < 3; ++r) {
		for (int c = 0; c < 3; ++c) {
			std::cout << get(c, r) << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix3::set(int col, int row, float value) {
	m_data[col * 3 + row] = value;
}

float Matrix3::det() const {
	Matrix2 top;
	Matrix2 mid;
	Matrix2 bot;

	for (int c = 0; c < 2; ++c) {
		for (int r = 0; r < 2; ++r) {
			top.set(c, r, get(c + 1, r));
			bot.set(c, r, get(c + 1, r + 1));
		}
	}

	mid.set(0, 0, get(1, 0));
	mid.set(1, 0, get(2, 0));
	mid.set(0, 1, get(1, 2));
	mid.set(1, 1, get(2, 2));

	return get(0, 0) * bot.det() - get(0, 1) * mid.det() + get(0, 2) * top.det();
}

float Matrix3::trace() const {
	return get(0, 0) + get(1, 1) + get(2, 2);
}

float Matrix3::get(int col, int row) const {
	return m_data[col * 3 + row];
}

bool Matrix3::isInvertible() const {
	return !util::equals(det(), 0.0f);
}

bool Matrix3::operator==(const Matrix3& right) const {
	for (int i = 0; i < 9; ++i) {
		if (!util::equals(m_data[i], right.m_data[i])) {
			return false;
		}
	}
	return true;
}

Matrix3 Matrix3::multiply(const Matrix3& right) const {
	Matrix3 result;
	float value;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			value = 0.0f;
			for (int k = 0; k < 3; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}
	return result;
}

Matrix3 Matrix3::inverse() const {
	float dt = det();

	if (util::equals(dt, 0.0f)) {
		return Matrix3(1.0f);
	}

	Matrix3 A2 = multiply(*this);
	float oneOverDet = 1.0f / dt;
	float trc = trace();
	float trcA2 = A2.trace();
	Matrix3 atrc = (*this) * trc;
	Matrix3 idttrc = Matrix3(0.5f * ((trc * trc) - trcA2));
	return (idttrc - atrc + A2) * oneOverDet;
}

Matrix3 Matrix3::transpose() const {
	Matrix3 trs;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			trs.set(i, j, get(j, i));
		}
	}
	return trs;
}

Matrix3 Matrix3::operator+(const Matrix3& right) const {
	Matrix3 result;
	for (int i = 0; i < 9; ++i) {
		result.m_data[i] = m_data[i] + right.m_data[i];
	}
	return result;
}

Matrix3 Matrix3::operator-(const Matrix3& right) const {
	Matrix3 result;
	for (int i = 0; i < 9; ++i) {
		result.m_data[i] = m_data[i] - right.m_data[i];
	}
	return result;
}

Matrix3 Matrix3::operator*(const Matrix3& right) const {
	return multiply(right);
}

Matrix3 Matrix3::operator*(float scalar) const {
	Matrix3 result;
	for (int i = 0; i < 9; ++i) {
		result.m_data[i] = m_data[i] * scalar;
	}
	return result;
}

Matrix3& Matrix3::operator+=(const Matrix3& right) {
	for (int i = 0; i < 9; ++i) {
		m_data[i] += right.m_data[i];
	}
	return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& right) {
	for (int i = 0; i < 9; ++i) {
		m_data[i] -= right.m_data[i];
	}
	return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& right) {
	Matrix3 result;
	float value;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			value = 0.0f;
			for (int k = 0; k < 3; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}

	for (int i = 0; i < 9; ++i) {
		m_data[i] = result.m_data[i];
	}
	return *this;
}

Matrix3& Matrix3::operator*=(float scalar) {
	for (int i = 0; i < 9; ++i) {
		m_data[i] *= scalar;
	}
	return *this;
}

Vector3 Matrix3::multiply(const Vector3& right) const {
	Vector3 res;
	res.x = right.x * get(0, 0) + right.y * get(1, 0) + right.z * get(2, 0);
	res.y = right.x * get(0, 1) + right.y * get(1, 1) + right.z * get(2, 1);
	res.z = right.x * get(0, 2) + right.y * get(1, 2) + right.z * get(2, 2);
	return res;
}

Vector3 Matrix3::operator*(const Vector3& right) const {
	return multiply(right);
}

Point3 Matrix3::multiply(const Point3& right) const {
	Point3 res;
	res.x = right.x * get(0, 0) + right.y * get(1, 0) + right.z * get(2, 0);
	res.y = right.x * get(0, 1) + right.y * get(1, 1) + right.z * get(2, 1);
	res.z = right.x * get(0, 2) + right.y * get(1, 2) + right.z * get(2, 2);
	return res;
}

Point3 Matrix3::operator*(const Point3& right) const {
	return multiply(right);
}

Matrix4::Matrix4() {
	memset(m_data, 0, sizeof(m_data));
}

Matrix4::Matrix4(const Matrix4& other) {
	for (int i = 0; i < 16; ++i) {
		m_data[i] = other.m_data[i];
	}
}

Matrix4::Matrix4(const float data[]) {
	for (int i = 0; i < 16; ++i) {
		m_data[i] = data[i];
	}
}

Matrix4::Matrix4(float diagonal) {
	memset(m_data, 0, sizeof(m_data));

	for (int i = 0; i < 4; ++i) {
		set(i, i, diagonal);
	}
}

Matrix4::Matrix4(float m00, float m10, float m20, float m30,
		float m01, float m11, float m21, float m31,
		float m02, float m12, float m22, float m32,
		float m03, float m13, float m23, float m33) {
	set(0, 0, m00);
	set(0, 1, m01);
	set(0, 2, m02);
	set(0, 3, m03);
	set(1, 0, m10);
	set(1, 1, m11);
	set(1, 2, m12);
	set(1, 3, m13);
	set(2, 0, m20);
	set(2, 1, m21);
	set(2, 2, m22);
	set(2, 3, m23);
	set(3, 0, m30);
	set(3, 1, m31);
	set(3, 2, m32);
	set(3, 3, m33);
}

void Matrix4::print() const {
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			std::cout << get(c, r) << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix4::set(int col, int row, float value) {
	m_data[col * 4 + row] = value;
}

float Matrix4::det() const {
	Matrix3 first;
	Matrix3 sec;
	Matrix3 thrd;
	Matrix3 fourth;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			first.set(i, j, get(i + 1, j + 1));
			
			if (j == 0) {
				sec.set(i, j, get(i + 1, j));
			}

			else {
				sec.set(i, j, get(i + 1, j + 1));
			}

			if (j == 0 || j == 1) {
				thrd.set(i, j, get(i + 1, j));
			}

			else {
				thrd.set(i, j, get(i + 1, j + 1));
			}

			fourth.set(i, j, get(i + 1, j));
		}
	}

	return get(0, 0) * first.det() - get(0, 1) * sec.det()
		+ get(0, 2) * thrd.det() - get(0, 3) * fourth.det();
}

float Matrix4::trace() const {
	return get(0, 0) + get(1, 1) + get(2, 2) + get(3, 3);
}

float Matrix4::get(int col, int row) const {
	return m_data[col * 4 + row];
}

bool Matrix4::isInvertible() const {
	return !util::equals(det(), 0.0f);
}

bool Matrix4::operator==(const Matrix4& right) const {
	for (int i = 0; i < 16; ++i) {
		if (!util::equals(m_data[i], right.m_data[i])) {
			return false;
		}
	}
	return true;
}

Matrix3 Matrix4::upperLeft() const {
	return Matrix3(
			get(0, 0), get(1, 0), get(2, 0),
			get(0, 1), get(1, 1), get(2, 1),
			get(0, 2), get(1, 2), get(2, 2)
		);
}

Matrix4 Matrix4::multiply(const Matrix4& right) const {
	Matrix4 result;
	float value;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			value = 0.0f;
			for (int k = 0; k < 4; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}
	return result;
}

Matrix4 Matrix4::inverse() const {
	float dt = det();

	if (util::equals(dt, 0.0f)) {
		return Matrix4(1.0f);
	}

	float oneOverDet = 1.0f / dt;
	float trcA = trace();
	float trc2A = trcA * trcA;
	float trc3A = trc2A * trcA;
	Matrix4 A2 = multiply(*this);
	Matrix4 A3 = A2.multiply(*this);

	float trcA2 = A2.trace();
	float trcA3 = A3.trace();
	
	Matrix4 firstPart = Matrix4(0.16667f * (trc3A - 3.0f * trcA * trcA2 + 2.0f * trcA3));
	Matrix4 secPart = (*this) * ((trc2A - trcA2) * 0.5f);
	Matrix4 thirdPart = A2 * trcA;
	return (firstPart - secPart + thirdPart - A3) * oneOverDet;
}

Matrix4 Matrix4::transpose() const {
	Matrix4 trs;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			trs.set(i, j, get(j, i));
		}
	}
	return trs;
}

Matrix4 Matrix4::operator+(const Matrix4& right) const {
	Matrix4 result;
	for (int i = 0; i < 16; i++) {
		result.m_data[i] = m_data[i] + right.m_data[i];
	}
	return result;
}

Matrix4 Matrix4::operator-(const Matrix4& right) const {
	Matrix4 result;
	for (int i = 0; i < 16; i++) {
		result.m_data[i] = m_data[i] - right.m_data[i];
	}
	return result;
}

Matrix4 Matrix4::operator*(const Matrix4& right) const {
	return multiply(right);
}

Matrix4 Matrix4::operator*(float scalar) const {
	Matrix4 result;
	for (int i = 0; i < 16; ++i) {
		result.m_data[i] = m_data[i] * scalar;
	}
	return result;
}

Matrix4& Matrix4::operator+=(const Matrix4& right) {
	for (int i = 0; i < 16; ++i) {
		m_data[i] += right.m_data[i];
	}
	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& right) {
	for (int i = 0; i < 16; ++i) {
		m_data[i] -= right.m_data[i];
	}
	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& right) {
	Matrix4 result;
	float value;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			value = 0.0f;
			for (int k = 0; k < 4; ++k) {
				value += get(k, j) * right.get(i, k);
			}
			result.set(i, j, value);
		}
	}

	for (int i = 0; i < 16; ++i) {
		m_data[i] = result.m_data[i];
	}

	return *this;
}

Matrix4& Matrix4::operator*=(float scalar) {
	for (int i = 0; i < 16; ++i) {
		m_data[i] *= scalar;
	}
	return *this;
}

Vector4 Matrix4::multiply(const Vector4& right) const {
	Vector4 result;
	result.x = right.x * get(0, 0) + right.y * get(1, 0) + right.z * get(2, 0) + right.w * get(3, 0);
	result.y = right.x * get(0, 1) + right.y * get(1, 1) + right.z * get(2, 1) + right.w * get(3, 1);
	result.z = right.x * get(0, 2) + right.y * get(1, 2) + right.z * get(2, 2) + right.w * get(3, 2);
	result.w = right.x * get(0, 3) + right.y * get(1, 3) + right.z * get(2, 3) + right.w * get(3, 3);
	return result;
}

Vector4 Matrix4::operator*(const Vector4& right) const {
	return multiply(right);
}

Point4 Matrix4::multiply(const Point4& right) const {
	Point4 result;
	result.x = right.x * get(0, 0) + right.y * get(1, 0) + right.z * get(2, 0) + right.w * get(3, 0);
	result.y = right.x * get(0, 1) + right.y * get(1, 1) + right.z * get(2, 1) + right.w * get(3, 1);
	result.z = right.x * get(0, 2) + right.y * get(1, 2) + right.z * get(2, 2) + right.w * get(3, 2);
	result.w = right.x * get(0, 3) + right.y * get(1, 3) + right.z * get(2, 3) + right.w * get(3, 3);
	return result;
}

Point4 Matrix4::operator*(const Point4& right) const {
	return multiply(right);
}

