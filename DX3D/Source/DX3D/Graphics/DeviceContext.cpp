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

void dx3d::DeviceContext::createVertexShader(ID3DBlob* shaderBlob)
{
	m_device.CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
}

void dx3d::DeviceContext::createPixelShader(ID3DBlob* shaderBlob)
{
	m_device.CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
}

void dx3d::DeviceContext::loadShaders()
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

ID3DBlob* dx3d::DeviceContext::loadShaderBlob(const WCHAR * path)
{
	ID3DBlob* shader_blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	DX3DGraphicsLogErrorAndThrow(D3DCompileFromFile(
		path,      // shader file name
		nullptr,                         // optional macros
		nullptr,                         // include interface
		"main",                          // entry point function
		"vs_5_0",                        // shader model
		0,                               // compile flags
		0,                               // effect flags
		&shader_blob,                    // compiled shader
		&error_blob                      // errors
	), "failed to extract shader.");
	if (error_blob != nullptr) {
		error_blob->Release();
	}
	return shader_blob;
}

ID3DBlob* dx3d::DeviceContext::loadPixelShaderBlob(const WCHAR* path)
{
	ID3DBlob* shader_blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	DX3DGraphicsLogErrorAndThrow(D3DCompileFromFile(
		path,      // shader file name
		nullptr,                         // optional macros
		nullptr,                         // include interface
		"main",                          // entry point function
		"ps_5_0",                        // shader model
		0,                               // compile flags
		0,                               // effect flags
		&shader_blob,                    // compiled shader
		&error_blob                      // errors
	), "failed to extract shader.");
	if (error_blob != nullptr) {
		error_blob->Release();
	}
	return shader_blob;
}
