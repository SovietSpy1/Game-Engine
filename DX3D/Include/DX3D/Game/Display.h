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
	private:
		UINT size_list;
		Microsoft::WRL::ComPtr<ID3DBlob> shader_blob;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelshader_blob;
		vertex list[6];
		Color startColV1;
		Color endColV1;
		Color startColV2;
		Color endColV2;
		Color startColV3;
		Color endColV3;
		Color currentCol;
		float u;
	};
}


