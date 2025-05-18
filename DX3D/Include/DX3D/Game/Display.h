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
		void UpdateQuadPosition();
		virtual ~Display() override;
	public:
		bool initialized = false;
	private:
		SwapChainPtr m_swapChain;
		std::shared_ptr<DeviceContext> m_device_context;
		std::shared_ptr<VertexBuffer> m_vb;
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<InputSystem> inputSystem;
	private:
		UINT size_list;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		std::shared_ptr<ConstantBuffer> constantBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::vector<vertex> list;
	private:
		Color currentCol{};
		float xRot{};
		float yRot{};
		float fov = PI;
		// Inherited via InputListener
		void onKeyDown(int key) override;
		void onKeyUp(int key) override;

		// Inherited via InputListener
		void onMouseMove(const Point& delta_mouse_pos) override;
	};
}



