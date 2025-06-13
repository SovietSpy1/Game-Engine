#pragma once
#include "DX3D/Core/Base.h"
#include <wrl.h>
namespace dx3d {
	class GameObject : public Base
	{
	public:
		GameObject(const BaseDesc& desc);
		void AddMesh(const wchar_t* file_path);
		void AddMesh();
		void AddMaterial();
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetAxis(float size = 1);
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Material> material;
		//Axis buffer for visualization
		bool showAxis = false;
		std::shared_ptr<Axis> axis{};
	public:
		float zRot{ 0.0f };
	};
}


