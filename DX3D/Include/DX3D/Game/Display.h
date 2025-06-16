#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Math/GameMath.h>
#include <wrl.h>
#include <DX3D/Input/InputListener.h>
#include <unordered_map>
namespace dx3d {
	class Display final: public Window, public InputListener
	{
	public:
		explicit Display(const DisplayDesc& desc);
		void onUpdate() override;
		void onFocus() override;
		void onKillFocus() override;
		std::shared_ptr<InputSystem>& getInputSystem() {
			return inputSystem;
		}
		static Display* Get();
		void GameObjectUpdate();
		void CameraUpdate();
		virtual ~Display() override;
		virtual void onResize(const Rect& new_size) override;
	public:
		bool initialized = false;
		bool pause = false;
		bool canPause = true;
		float u = 0.0f;
	private:
		SwapChainPtr m_swapChain;
		std::shared_ptr<DeviceContext> m_device_context;
		std::shared_ptr<InputSystem> inputSystem;
		std::shared_ptr<ConstantBuffer> constantBuffer;
		std::unordered_map<UINT, std::shared_ptr<ConstantBuffer>> constantBuffers{};
	private:
		static inline Display* S{};
		std::shared_ptr<Camera> camera;
	private:
		Color currentCol{};
		float fov = PI /4.0f;
		float aspectRatio = 1.0f;
		ConstantBufferDesc cBuff{};
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



