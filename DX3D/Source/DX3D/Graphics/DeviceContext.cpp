#include "DX3D/Graphics/DeviceContext.h"
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <d3dcompiler.h>
dx3d::DeviceContext::DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_device_context(desc.context)
{
}

void dx3d::DeviceContext::clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain, vec4 rgba)
{
	FLOAT clear_color[] = { rgba.r, rgba.g, rgba.b, rgba.a};
	m_device_context.ClearRenderTargetView(swap_chain->m_rtv.Get(), clear_color);
	m_device_context.OMSetRenderTargets(1, swap_chain->m_rtv.GetAddressOf(), NULL);
}

void dx3d::DeviceContext::setVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context.IASetVertexBuffers(0, 1, vertex_buffer->m_buffer.GetAddressOf(), &stride, &offset);
	m_device_context.IASetInputLayout(vertex_buffer->m_layout.Get());
}

void dx3d::DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context.Draw(vertex_count, start_vertex_index);
}

void dx3d::DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context.Draw(vertex_count, start_vertex_index);
}

void dx3d::DeviceContext::SetViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context.RSSetViewports(1, &vp);
}

void dx3d::DeviceContext::loadShaders(Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader)
{
	m_device_context.VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_device_context.PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void dx3d::DeviceContext::createTransparentBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_device.CreateBlendState(&blendDesc, &blendState);
}

void dx3d::DeviceContext::setBlendState()
{
	float blendFactor[4] = { 0,0,0,0 };
	UINT sample_mask = 0xffffffff;
	m_device_context.OMSetBlendState(blendState.Get(), blendFactor, sample_mask);
}

void dx3d::DeviceContext::createBackfaceRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE; // Disables backface culling
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthClipEnable = true;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateRasterizerState(&rasterDesc, rasterizerState.GetAddressOf()), "rasterstate creation failed.");
}

void dx3d::DeviceContext::setRasterState()
{
	m_device_context.RSSetState(rasterizerState.Get());
}

void dx3d::DeviceContext::createConstantBuffer()
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(ConstantBuffer);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData{};
	ConstantBuffer cBuffer{};
	initData.pSysMem = &cBuffer;
	m_device.CreateBuffer(&desc, &initData, constantBuffer.GetAddressOf());
}
void dx3d::DeviceContext::setConstantBuffer(ConstantBuffer cBuffer)
{
	m_device_context.UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cBuffer, 0, 0);
	m_device_context.PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
