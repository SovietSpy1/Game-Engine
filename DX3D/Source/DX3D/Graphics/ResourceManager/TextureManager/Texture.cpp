#include "DX3D/Graphics/ResourceManager/TextureManager/Texture.h"
#include <DirectXTex.h>
#include "DX3D/Graphics/GraphicsEngine.h"
#include <iostream>
#include <DX3D/Game/Display.h>
#include <DX3D/Graphics/DeviceContext.h>
dx3d::Texture::Texture(const wchar_t* full_path, const GraphicsResourceDesc& desc) : Resource(full_path, desc)
{
	DirectX::ScratchImage image_data;
	DX3DGraphicsLogErrorAndThrow(DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data), "LoadTextureFromWICFile failed.");
	DX3DGraphicsLogErrorAndThrow(DirectX::CreateTexture(&m_device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), m_texture.GetAddressOf()), "CreateTexture failed.");
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = image_data.GetMetadata().format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(m_texture.Get(), &srv_desc, m_srv.GetAddressOf()), "Create shaderResourceView failed.");

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, m_sampler.GetAddressOf()), "CreateSamplerState failed.");
}

dx3d::Texture::Texture(int resolution, const GraphicsResourceDesc& desc) : Resource(desc)
{
	std::vector<vec4_32> initColors(resolution * resolution, vec4_32{0,0,0,0});
	D3D11_SUBRESOURCE_DATA initValues{};
	initValues.pSysMem = initColors.data();
	initValues.SysMemPitch = resolution * sizeof(vec4_32);
	initValues.SysMemSlicePitch = 0;
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Height = resolution;
	texDesc.Width = resolution;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DYNAMIC;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texDesc.MiscFlags = 0;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture{};
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture2D(&texDesc, &initValues, texture.GetAddressOf()), "Create Smoke Texture Failed!");
	m_texture = texture;

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc{};
	shader_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_desc.Texture2D.MipLevels = 1;
	shader_desc.Texture2D.MostDetailedMip = 0;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(m_texture.Get(), &shader_desc, m_srv.GetAddressOf()), "Create shaderResourceView failed.");

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, m_sampler.GetAddressOf()), "CreateSamplerState failed.");
}

void dx3d::Texture::MapToTexture(std::vector<vec4_32> data, int resolution)
{
	D3D11_MAPPED_SUBRESOURCE map{};
	map = Display::get()->m_device_context->GetMap(m_texture);
	uint8_t* destPtr = reinterpret_cast<uint8_t*>(map.pData);
	const uint8_t* sourcePtr = reinterpret_cast<uint8_t*>(data.data());
	UINT pitch = 4 * resolution;
	for (UINT y = 0; y < resolution; y++) {
		::memcpy(destPtr + y * map.RowPitch, sourcePtr + y * pitch, pitch);
	}
	Display::get()->m_device_context->UnMap(m_texture);
}
	
