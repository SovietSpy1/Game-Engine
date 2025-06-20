#pragma once
#include <DX3D/Object/GameObject.h>
#include <DX3D/Object/Transform.h>
#include <DX3D/Object/Objects/Camera.h>
namespace dx3d {
	class SkyBox : public GameObject {
	public:
		SkyBox(const BaseDesc& desc, Camera* camera) : GameObject(desc), cam(camera){};
		void Update() override {

			GetComponent<Transform>()->position.SetTranslate(cam->GetComponent<Transform>()->GetPosition());
		}
		Camera* cam;
	};
}