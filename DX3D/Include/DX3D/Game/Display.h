#pragma once
#include <DX3D/Window/Window.h>
#include <DX3D/Math/GameMath.h>
#include <wrl.h>
namespace dx3d {
	class Display final: public Window
	{
	public:
		explicit Display(const DisplayDesc& desc);
		virtual void onUpdate() override;
		virtual ~Display() override;
	private:
		SwapChainPtr m_swapChain;
		std::shared_ptr<DeviceContext> m_device_context;
		std::shared_ptr<VertexBuffer> m_vb;
		std::shared_ptr<Mesh> m_mesh;
	private:
		UINT size_list;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		std::vector<vertex> list;
		Color startColV1;
		Color currentCol;
		float u;
	};
}


