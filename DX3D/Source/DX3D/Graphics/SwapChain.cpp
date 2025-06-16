#include <DX3D/Graphics/SwapChain.h>
dx3d::SwapChain::SwapChain(const SwapChainDesc& desc, const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc), m_windowHandle{static_cast<HWND>(desc.winHandle)}, m_windowSize{desc.winSize}
{
	DXGI_SWAP_CHAIN_DESC dxgiDesc{};
	dxgiDesc.BufferDesc.Width = std::max(1, m_windowSize.width);
	dxgiDesc.BufferDesc.Height = std::max(1, m_windowSize.height);
	dxgiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiDesc.BufferCount = 2;
	dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	dxgiDesc.OutputWindow = m_windowHandle;
	dxgiDesc.SampleDesc.Count = 1;
	dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiDesc.Windowed = TRUE;
	DX3DGraphicsLogErrorAndThrow(m_factory.CreateSwapChain(&m_device, &dxgiDesc, m_swapChain.GetAddressOf()), "CreateSwapChain failed.");
	createRenderTargetView();
}
void dx3d::SwapChain::createRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer{};
	DX3DGraphicsLogErrorAndThrow(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)buffer.GetAddressOf()), "GetBackBuffer failed.");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateRenderTargetView(buffer.Get(), NULL, m_rtv.GetAddressOf()), "CreateRenderTargetView failed.");
	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Width = m_windowSize.width;
	tex_desc.Height = m_windowSize.height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture2D(&tex_desc, nullptr, depthBuffer.GetAddressOf()), "Create depth buffer texture failed.");
	DX3DGraphicsLogErrorAndThrow(m_device.CreateDepthStencilView(depthBuffer.Get(), NULL, m_dsv.GetAddressOf()), "Create Depth Stencil View Failed.");
}
bool dx3d::SwapChain::present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
	return true;
}

void dx3d::SwapChain::resize(UINT width, UINT height)
{
	m_rtv->Release();
	m_dsv->Release();
	m_swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	m_windowSize = Rect(width, height);
	createRenderTargetView();
}
