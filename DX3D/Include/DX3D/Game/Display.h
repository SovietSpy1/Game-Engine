#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Math/GameMath.h>
#include <wrl.h>
#include <DX3D/Input/InputListener.h>
#include <unordered_map>
namespace dx3d {
	class Display final: public Window
	{
	public:
		explicit Display(const DisplayDesc& desc);
		void Update();
		void CameraUpdate(Matrix4X4 lightRot, Camera* cam, float fov);
		void onFocus() override;
		void onKillFocus() override;
		void Draw(std::shared_ptr<GameObject> currentObject);
		static Display* get();
		virtual ~Display() override;
		virtual void onResize(const Rect& new_size) override;
	public:
		bool pause = false;
	private:
		SwapChainPtr m_swapChain;
		std::shared_ptr<DeviceContext> m_device_context;
		std::unique_ptr<GameManager> gameManager;
	private:
		static inline Display* S{};
		ConstantBufferDesc cBuff{};
		std::unordered_map<UINT, std::shared_ptr<ConstantBuffer>> constantBuffers{};
		std::shared_ptr<ConstantBuffer> constantBuffer;
		friend class Texture;
		friend class InputSystem;
	private:
		vec4_32 currentCol{};
		float aspectRatio = 1.0f;
		
	};
}



