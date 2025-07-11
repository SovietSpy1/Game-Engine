#pragma once
#include <DX3D/Object/GameObject.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Object/Transform.h>
#include <DX3D/Game/Display.h>
namespace dx3d {
	class Player : public GameObject {
	public:
		Player(const BaseDesc& desc) : GameObject(desc) {}
		void Update() override {
			RigidBody* rigidBody = GetComponent<RigidBody>();
			Transform* transform = GetComponent<Transform>();
			if (rigidBody != nullptr) {
				Vector3D moveDir = transform->rotation.getZDirection() * forward + transform->rotation.getXDirection() * rightward;
				rigidBody->SetVelocity(Vector3D(velocity * moveDir.x, rigidBody->velocity.y, velocity * moveDir.z));
			}
			transform->rotation.SetIdentity();
			Matrix4X4 temp{};
			temp.SetRotationY(yRot);
			transform->rotation *= temp;
		}
		void Jump() {
			RigidBody* rigidBody = GetComponent<RigidBody>();
			if (rigidBody != nullptr) {
				rigidBody->AddForce(Vector3D::up() * 200);
			}
		}
		void onKeyDown(int key) override {
			if (key == 'D') {
				rightward = 1.0f;
			}
			if (key == 'A') {
				rightward = -1;
			}
			if (key == 'W') {
				forward = 1;
			}
			if (key == 'S') {
				forward = -1;
			}
		}
		void onKeyDownFirst(int key) {
			if (key == VK_SHIFT) {
				RigidBody* rb = GetComponent<RigidBody>();
				if (rb != nullptr) {
					Jump();
				}
			}
		}
		void onKeyUp(int key) override {
			forward = 0;
			rightward = 0;
		}
		void onMouseMove(const Point& mouse_pos) override {
			if (lastPos.x == 0 && lastPos.y == 0) {
				lastPos = mouse_pos;
			}
			float deltaX = mouse_pos.x - lastPos.x;
			yRot += deltaX * 0.001f;
		}
		Point lastPos{0,0};
		float forward = 0.0f;
		float rightward = 0.0f;
		float velocity = 2;
		float xRot{};
		float yRot{};
	};
}