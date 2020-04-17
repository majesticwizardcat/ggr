#include "entities/entity.h"

Entity::Entity() { }
Entity::Entity(const Entity& other) : m_mesh(other.m_mesh), m_material(other.m_material) { }
Entity::Entity(const std::shared_ptr<Triangle>& mesh, const std::shared_ptr<Material>& material, int meshID) :
	m_mesh(mesh), m_material(material) {
	m_mesh->id = meshID;
}

void Entity::intersects(const Ray& ray, float maxT, Intersection* result) const {
	if (m_mesh->intersects(ray, maxT, result)) {
		result->hit = true;
		result->material = m_material;
		result->light = nullptr;
	}
}

bool Entity::intersects(const Ray& ray, float maxT) const {
	return m_mesh->intersects(ray, maxT);
}

int Entity::getMeshID() const {
	return m_mesh->id;
}

