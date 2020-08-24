#pragma once

class Normal;

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;

class Normal : public Vector3 {
public:
	Normal() : Vector3() { }
	Normal(const Vector3& v) : Vector3(v) { }
	Normal(const Vector4& v) : Vector3(v) { }
};

