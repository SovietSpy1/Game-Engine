#pragma once
#include <DX3D/Object/GameObject.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Object/Transform.h>
#include <DX3D/Game/Display.h>
namespace dx3d {
	class Camera : public GameObject {
	public:
		Camera(const BaseDesc& desc) : GameObject(desc) {}
		void Update() override {
		}
		void onKeyDown(int key) override {
		}
		void onKeyUp(int key) override {
		}
		void onMouseMove(const Point& mouse_pos) override {
			InputSystem::get()->setCursorPosition(Point(Display::get()->m_size.width / 2.0f, Display::get()->m_size.height / 2.0f));
			xRot -= (mouse_pos.y - (Display::get()->m_size.height / 2.0f)) * Time::deltaTime * 0.1f;
			yRot -= (mouse_pos.x - (Display::get()->m_size.width / 2.0f)) * Time::deltaTime * 0.1f;
			if (xRot > PI / 4.0f) xRot = PI / 4.0f;
			if (xRot < -PI / 4.0f) xRot = -PI / 4.0f;
			Transform* transform = GetComponent<Transform>();
			transform->rotation.SetIdentity();
			Matrix4X4 temp{};
			temp.SetRotationX(xRot);
			transform->rotation *= temp;
		}
		float xRot{};
		float yRot{};
	};
}