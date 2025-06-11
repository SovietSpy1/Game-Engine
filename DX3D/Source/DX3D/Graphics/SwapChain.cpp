#include <DX3D/Graphics/SwapChain.h>
dx3d::SwapChain::SwapChain(const SwapChainDesc& desc, const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc)
{
	DXGI_SWAP_CHAIN_DESC dxgiDesc{};
	dxgiDesc.BufferDesc.Width = std::max(1, desc.winSize.width);
	dxgiDesc.BufferDesc.Height = std::max(1, desc.winSize.height);
	dxgiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiDesc.BufferCount = 2;
	dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	dxgiDesc.OutputWindow = static_cast<HWND>(desc.winHandle);
	dxgiDesc.SampleDesc.Count = 1;
	dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiDesc.Windowed = TRUE;
	DX3DGraphicsLogErrorAndThrow(m_factory.CreateSwapChain(&m_device, &dxgiDesc, m_swapChain.GetAddressOf()), "CreateSwapChain failed.");
	ID3D11Texture2D* buffer{};
	DX3DGraphicsLogErrorAndThrow(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer), "GetBackBuffer failed.");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateRenderTargetView(buffer, NULL, m_rtv.GetAddressOf()), "CreateRenderTargetView failed.");
	buffer->Release();
	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Width = desc.winSize.width;
	tex_desc.Height = desc.winSize.height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture2D(&tex_desc, nullptr, &buffer), "Create depth buffer texture failed.");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateDepthStencilView(buffer, NULL, &m_dsv), "Create Depth Stencil View Failed.");


}

bool dx3d::SwapChain::present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
	return true;
}
