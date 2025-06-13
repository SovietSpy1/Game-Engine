#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Math/GameMath.h>
#include <wrl.h>
#include <DX3D/Input/InputListener.h>
namespace dx3d {
	class Display final: public Window, public InputListener
	{
	public:
		explicit Display(const DisplayDesc& desc);
		virtual void onUpdate() override;
		virtual void onFocus() override;
		virtual void onKillFocus() override;
		std::shared_ptr<InputSystem>& getInputSystem() {
			return inputSystem;
		}
		void GameObjectUpdate();
		void CameraUpdate();
		void SkyBoxUpdate();
		virtual ~Display() override;
	public:
		bool initialized = false;
		bool pause = false;
		bool canPause = true;
		float u = 0.0f;
	private:
		SwapChainPtr m_swapChain;
		std::shared_ptr<DeviceContext> m_device_context;
		std::shared_ptr<InputSystem> inputSystem;
	private:
		std::shared_ptr<ConstantBuffer> constantBuffer;
	private:
		std::shared_ptr<GameObject> skyBox;
		Color currentCol{};
		float xRot{};
		float yRot{};
		float fov = PI /4.0f;
		float aspectRatio = 1.0f;	
		Vector3D scale = Vector3D(1.0f,1.0f,1.0f);
		Matrix4X4 m_worldCam{};
		ConstantBufferDesc cBuff{};
		float forward = 0.0f;
		float rightward = 0.0f;
		std::shared_ptr<Texture> woodTexture = nullptr;
		std::shared_ptr<GameObject> currentObject;
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		float lightRotation = 0.0f;
	private:
		// Inherited via InputListener
		void onKeyDown(int key) override;
		void onKeyUp(int key) override;
		void onKeyDownFirst(int key) override;
		// Inherited via InputListener
		void onMouseMove(const Point& mouse_pos) override;
		void onLeftMouseDown(const Point& mouse_pos) override;
		void onRightMouseDown(const Point& mouse_pos) override;
	};
}



