#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
namespace dx3d {
	class SwapChain final: public GraphicsResource
	{
	public:
		explicit SwapChain(const SwapChainDesc& desc, const GraphicsResourceDesc& gDesc);
		void createRenderTargetView();
		bool present(bool vysnc);
		void resize(UINT width, UINT height);
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv{};
		HWND m_windowHandle{};
		Rect m_windowSize{};
	private:
		friend class DeviceContext;
		friend class Display;
	};
}


