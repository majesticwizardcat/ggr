#include "entities/entity.h"

Entity::Entity(const Entity& other) : Entity(other.m_mesh, other.m_material, other.m_id) { }
Entity::Entity(const Triangle* mesh, const Material* material, int id) : m_mesh(mesh),
	m_material(material), m_id(id) { }

void Entity::intersects(const Ray& ray, float maxT, Intersection* result) const {
	if (m_mesh->intersects(ray, maxT, result)) {
		result->hit = true;
		result->material = m_material;
		result->light = nullptr;
		result->intersectionPoint.meshID = m_id;
	}
}

bool Entity::intersects(const Ray& ray, float maxT) const {
	return m_mesh->intersects(ray, maxT);
}

int Entity::getID() const {
	return m_id;
}

BoundingBox Entity::createBoundingBox() const {
	return BoundingBox(*m_mesh);
}

