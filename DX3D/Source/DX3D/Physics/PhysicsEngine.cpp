#include "DX3D/Physics/PhysicsEngine.h"
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/Collider.h>
#include <DX3D/Object/GameObject.h>
#include <DX3D/Object/Transform.h>
#include <cfloat>
#include <cmath>
dx3d::PhysicsEngine::PhysicsEngine(const BaseDesc& desc) : Base(desc)
{
	if (S == nullptr) {
		S = this;
	}
	else {
		DX3DLogError("PhysicsEngine already initialized, cannot initialize again.");
	}
}

dx3d::PhysicsEngine* dx3d::PhysicsEngine::get()
{
	return S;
}

void dx3d::PhysicsEngine::Update()
{
	// Update physics simulation logic here
	// This could include updating positions, checking collisions, etc.
	for (auto& rigidBody : rigidBodies) {
		if (rigidBody->gravity && !rigidBody->grounded) {
			rigidBody->AddForce(Vector3D::up() * gravity);
		}
		rigidBody->Update();
	}
	for (auto& collider : colliders) {
		Transform* transform = collider->owner->GetComponent<Transform>();
		for (size_t i = 0; i < collider->baseVertices.size(); i++) {
			Vector3D transformedPos = collider->scale.ReverseMul(collider->baseVertices.at(i));
			transformedPos = transform->Get().ReverseMul(transformedPos);
			transformedPos = collider->transform.ReverseMul(transformedPos);
			collider->vertices.at(i) = transformedPos;
		}
		for (size_t i = 0; i < collider->baseNormals.size(); i++) {

			collider->normals.at(i) = transform->rotation.ReverseMul(collider->baseNormals.at(i));
		}
		for (size_t i = 0; i < collider->baseEdges.size(); i++) {
			collider->edges.at(i) = transform->rotation.ReverseMul(collider->baseEdges.at(i));
		}
	}
	for (const auto& collider : colliders) {
		for (const auto& otherCollider : colliders) {
			if (collider != otherCollider) {
				Vector3D directionToMove{};
				RigidBody* rigidBody = collider->owner->GetComponent<RigidBody>();
				RigidBody* otherRb = otherCollider->owner->GetComponent<RigidBody>();
				if (IntersectCheck(collider, otherCollider, &directionToMove)) {
					if (rigidBody != nullptr) {
						switch (rigidBody->physicsType) {
						case PhysicsType::Kinematic:
							rigidBody->onCollide();
							//nothing happens
							break;
						case PhysicsType::Dynamic:
							if (otherRb == nullptr) {
								rigidBody->onCollideDynamic(directionToMove);
								break;
							}
							switch (otherRb->physicsType) {
							case PhysicsType::Kinematic:
								rigidBody->onCollideDynamic(directionToMove);
								break;
							case PhysicsType::Dynamic:
								rigidBody->onCollideDynamic(directionToMove * 0.5f);
								break;
							}
							break;
						default:
							DX3DLogWarning("Not a valid PhysicsType");
						}
					}
				}
				else {
					if (rigidBody != nullptr) {
						if (rigidBody->colliding) {
							rigidBody->onCollideEnd();
						}
					}
				}
			}
		}
	}
}

bool dx3d::PhysicsEngine::IntersectCheck(const std::shared_ptr<Collider>& collider1, const std::shared_ptr<Collider>& collider2, Vector3D* moveVec)
{
	std::vector<Vector3D> vertices1 = collider1->vertices;
	std::vector<Vector3D> vertices2 = collider2->vertices;
	std::vector<Vector3D> normals1 = collider1->normals;
	std::vector<Vector3D> normals2 = collider2->normals;
	std::vector<Vector3D> edges1 = collider1->edges;
	std::vector<Vector3D> edges2 = collider2->edges;
	Vector3D leastDistAxis;
	float leastDist = FLT_MAX; // Initialize to maximum value
	if (vertices1.empty() || vertices2.empty() || normals1.empty() || normals2.empty()) {
		return false;
	}
			auto checkAxis = [&](const auto& axis) {
		float min1 = FLT_MAX, min2 = FLT_MAX, max1 = -FLT_MAX, max2 = -FLT_MAX;
		for (const auto& v : vertices1) {
			float d = axis.dot(v);
			min1 = std::min(min1, d);
			max1 = std::max(max1, d);
		}
		for (const auto& v : vertices2) {
			float d = axis.dot(v);
			min2 = std::min(min2, d);
			max2 = std::max(max2, d);
		}
		
		// Use consistent floating-point comparison with epsilon
		const float epsilon = 0.0001f;
		if (!(max1 + epsilon < min2 || max2 + epsilon < min1)) {
			float least;
			float dist = min2 - max1;
			float dist2 = max2 - min1;
			if (std::abs(dist) > std::abs(dist2)) {
				least = dist2;
			}
			else {
				least = dist;
			}
			if (std::abs(least) < std::abs(leastDist)) {
				leastDist = least;
				leastDistAxis = axis;
			}
			return true;
		}
		else {
			return false;
		}
	};
	for (const auto& n : normals1) {
		if (!checkAxis(n)) return false;
	}
	for (const auto& n : normals2) {
		if (!checkAxis(n)) return false;
	}
	std::vector<Vector3D> crossedEdges;
	for (const auto& e1 : edges1) {
		for (const auto& e2 : edges2) {
			Vector3D crossedEdge = e1.cross(e2);
			// Use consistent floating-point comparison
			if (crossedEdge.mag() >= 0.001f) {
				Vector3D normalized = crossedEdge.normalize();
				// Check for valid normalized vector
				if (normalized.mag() > 0.5f) {
					crossedEdges.push_back(normalized);
				}
			}
		}
	}
	for (const auto& e : crossedEdges) {
		if (!checkAxis(e)) return false;
	}
	if (moveVec != nullptr) {
		*moveVec = leastDistAxis * leastDist;
	}
	return true;
}

std::shared_ptr<dx3d::RigidBody> dx3d::PhysicsEngine::createRigidBody(GameObject* owner)
{
	std::shared_ptr rigidBody = std::make_shared<RigidBody>(BaseDesc{ m_logger }, owner);
	rigidBodies.push_back(rigidBody);
	return rigidBody;
}

std::shared_ptr<dx3d::Collider> dx3d::PhysicsEngine::createCollider(GameObject* owner, ColliderType colliderType)
{
	std::shared_ptr<Collider> collider = std::make_shared<Collider>(BaseDesc{ m_logger }, owner, colliderType);
	colliders.push_back(collider);
	return collider;
}
