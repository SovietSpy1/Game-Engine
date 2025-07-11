#pragma once
#include <DX3D/Core/Base.h>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <string>
#include <bitset>
#include <cassert>
#include <DX3D/Game/Display.h>

namespace dx3d {
	class GameManager : public Base
	{
	public:
		//constructor
		GameManager(const BaseDesc& desc);
		void Update();
		void GameObjectUpdate();
		void CameraUpdate();
		void LoadBasicScene();
		void LoadSmokeScene();
		void SmokeTest2DUpdate();
		static GameManager* get() {
			return S;
		}
	private:
		static inline GameManager* S{};
		float fov = PI / 4.0f;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<GameObject> currentObject;
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::shared_ptr<Smoke> smokeHolder;
		float lightRotation = 0.0f;
		friend class Display;
	};

	
}