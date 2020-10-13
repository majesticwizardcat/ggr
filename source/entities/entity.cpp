#include "entities/entity.h"

void Entity::fillMeshIntersection(float w0, float w1, float w2, Intersection* result) const {
	m_mesh->fillIntersection(w0, w1, w2, result);
}

BoundingBox Entity::createBoundingBox() const { return BoundingBox(*m_mesh); }

bool Entity::intersects(const Ray& ray, float maxT, EntityIntersection* result) const {
	if (m_mesh->intersects(ray, maxT, result)) {
		if (result) {
			result->entity = this;
		}
		return true;
	}
	return false;
}
