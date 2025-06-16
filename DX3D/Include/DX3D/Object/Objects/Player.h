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
			if (rigidBody != nullptr) {
				Vector3D moveDir = transform->rotation.getZDirection() * forward + transform->rotation.getXDirection() * rightward;
				rigidBody->SetVelocity(Vector3D(velocity * moveDir.x, rigidBody->velocity.y, velocity * moveDir.z));
			}
		}
		void Jump() {
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
				Jump();
			}
		}
		void onKeyUp(int key) override {
			forward = 0;
			rightward = 0;
		}
		void onMouseMove(const Point& mouse_pos) override {
			InputSystem::get()->setCursorPosition(Point(Display::Get()->m_size.width / 2.0f, Display::Get()->m_size.height / 2.0f));
			xRot -= (mouse_pos.y - (Display::Get()->m_size.height / 2.0f)) * Time::deltaTime * 0.1f;
			yRot -= (mouse_pos.x - (Display::Get()->m_size.width / 2.0f)) * Time::deltaTime * 0.1f;
			if (xRot > PI / 2.0f) xRot = PI / 2.0f;
			if (xRot < -PI / 2.0f) xRot = -PI / 2.0f;
			transform->rotation.SetIdentity();
			Matrix4X4 temp{};
			temp.SetRotationY(yRot);
			transform->rotation *= temp;
		}
		float forward = 1.0f;
		float rightward = 1.0f;
		float velocity = 2;
		float xRot{};
		float yRot{};
	};
}