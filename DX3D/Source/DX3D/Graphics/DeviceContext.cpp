#include "DX3D/Graphics/DeviceContext.h"
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <d3dcompiler.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/Texture.h>
dx3d::DeviceContext::DeviceContext(const DeviceContextDesc& desc, const GraphicsResourceDesc& gDesc) : GraphicsResource(gDesc), m_device_context(desc.context)
{
	if (S == nullptr) {
		S = this;
	}
}

void dx3d::DeviceContext::clearRenderTargetColor(std::shared_ptr<SwapChain> swap_chain, vec4_32 rgba)
{
	FLOAT clear_color[] = { rgba.r/255.0f, rgba.g/255.0f, rgba.b/255.0f, rgba.a/255.0f};
	m_device_context.ClearRenderTargetView(swap_chain->m_rtv.Get(), clear_color);
	m_device_context.ClearDepthStencilView(swap_chain->m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_device_context.OMSetRenderTargets(1, swap_chain->m_rtv.GetAddressOf(), swap_chain->m_dsv.Get());
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

void dx3d::DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context.DrawIndexed(index_count, start_vertex_index, start_index_location);
}

void dx3d::DeviceContext::DrawLines(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_device_context.Draw(vertex_count, start_vertex_index);
}

void dx3d::DeviceContext::DrawIndexedLines(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_device_context.DrawIndexed(index_count, start_vertex_index, start_index_location);
}

void dx3d::DeviceContext::SetViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context.RSSetViewports(1, &vp);
}

void dx3d::DeviceContext::loadShaders(Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader)
{
	m_device_context.VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_device_context.PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

D3D11_MAPPED_SUBRESOURCE dx3d::DeviceContext::GetMap(Microsoft::WRL::ComPtr<ID3D11Resource> resource)
{
	D3D11_MAPPED_SUBRESOURCE mapped;
	m_device_context.Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	return mapped;
}

D3D11_MAPPED_SUBRESOURCE dx3d::DeviceContext::GetReadableMap(Microsoft::WRL::ComPtr<ID3D11Resource> resource)
{
	D3D11_MAPPED_SUBRESOURCE mapped;
	m_device_context.Map(resource.Get(), 0, D3D11_MAP_READ, 0, &mapped);
	return mapped;
}

void dx3d::DeviceContext::UnMap(Microsoft::WRL::ComPtr<ID3D11Resource> resource)
{
	m_device_context.Unmap(resource.Get(), 0);
}

void dx3d::DeviceContext::CSSetSRVS(std::vector<ID3D11ShaderResourceView*> srvs)
{
	m_device_context.CSSetShaderResources(0, (UINT)srvs.size(), srvs.data());
}

void dx3d::DeviceContext::CSSetUAVS(std::vector<ID3D11UnorderedAccessView*> uavs)
{
	m_device_context.CSSetUnorderedAccessViews(0, (UINT)uavs.size(), uavs.data(), nullptr);
}

void dx3d::DeviceContext::CSSetConstantBuffers(const std::unordered_map<UINT, std::shared_ptr<ConstantBuffer>> buffers)
{
	for (auto it = buffers.begin(); it != buffers.end(); it++) {
		m_device_context.CSSetConstantBuffers(it->first, 1, it->second->m_buffer.GetAddressOf());
	}
}

void dx3d::DeviceContext::UpdateSubresource(ID3D11Resource* resource, void* subData)
{
	m_device_context.UpdateSubresource(resource, 0, nullptr, subData, 0, 0);
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

void dx3d::DeviceContext::Dispatch(uint32_t xGroups, uint32_t yGroups, uint32_t zGroups)
{
	m_device_context.Dispatch(xGroups, yGroups, zGroups);
}

void dx3d::DeviceContext::CSSetShader(Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader)
{
	m_device_context.CSSetShader(m_computeShader.Get(), nullptr, 0);
}

void dx3d::DeviceContext::createBackfaceRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT; // Disables backface culling
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthClipEnable = true;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateRasterizerState(&rasterDesc, backFaceRasterizerState.GetAddressOf()), "rasterstate creation failed.");
}

void dx3d::DeviceContext::createFrontfaceRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK; // Disables backface culling
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthClipEnable = true;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateRasterizerState(&rasterDesc, frontFaceRasterizerState.GetAddressOf()), "rasterstate creation failed.");
}

void dx3d::DeviceContext::createAllfaceRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE; // Disables backface culling
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthClipEnable = true;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateRasterizerState(&rasterDesc, allFaceRasterizerState.GetAddressOf()), "rasterstate creation failed.");
}

void dx3d::DeviceContext::setRasterizerState(dx3d::RasterizerStateType rasterizerType)
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	switch (rasterizerType)
	{
	case dx3d::RasterizerStateType::BackFace:
		if (!backFaceRasterizerState) {
			createBackfaceRasterizerState();
		}
		rasterizerState = backFaceRasterizerState;
		break;
	case dx3d::RasterizerStateType::FrontFace:
		if (!frontFaceRasterizerState) {
			createFrontfaceRasterizerState();
		}
		rasterizerState = frontFaceRasterizerState;
		break;
	case dx3d::RasterizerStateType::AllFace:
		if (!allFaceRasterizerState) {
			createAllfaceRasterizerState();
		}
		rasterizerState = allFaceRasterizerState;
		break;
	default:
		DX3DLogErrorAndThrow("Invalid rasterizer state type specified.");
	}
	m_device_context.RSSetState(rasterizerState.Get());
}
void dx3d::DeviceContext::setConstantBuffers(const std::unordered_map<UINT, std::shared_ptr<ConstantBuffer>> buffers)
{
	for (auto it = buffers.begin(); it != buffers.end(); it++) {
		m_device_context.PSSetConstantBuffers(it->first, 1, it->second->m_buffer.GetAddressOf());
		m_device_context.VSSetConstantBuffers(it->first, 1, it->second->m_buffer.GetAddressOf());
	}
}


void dx3d::DeviceContext::setIndexBuffer(std::shared_ptr<IndexBuffer> iBuffer)
{
	m_device_context.IASetIndexBuffer(iBuffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void dx3d::DeviceContext::setTexture(std::vector<std::shared_ptr<Texture>> textures)
{
	ID3D11ShaderResourceView* srvs[32];
	ID3D11SamplerState* samplers[32];
	for (size_t i = 0; i < textures.size(); ++i) {
		srvs[i] = textures[i]->m_srv.Get();
		samplers[i] = textures[i]->m_sampler.Get();
	}

	m_device_context.PSSetShaderResources(0, (UINT)textures.size(), srvs);
	m_device_context.PSSetSamplers(0, (UINT)textures.size(), samplers);
}

void dx3d::DeviceContext::PSSetSRVS(std::vector<ID3D11ShaderResourceView*> srvs)
{
	m_device_context.PSSetShaderResources(1, (UINT)srvs.size(), srvs.data());
}
