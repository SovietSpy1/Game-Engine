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
	DX3DGraphicsLogErrorAndThrow(DirectX::CreateTexture(&m_device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), texture.GetAddressOf()), "CreateTexture failed.");
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = image_data.GetMetadata().format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(texture.Get(), &srv_desc, srv.GetAddressOf()), "Create shaderResourceView failed.");

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, sampler.GetAddressOf()), "CreateSamplerState failed.");
}

dx3d::Texture::Texture(const GraphicsResourceDesc& desc) : Resource(desc)
{
	
}

void dx3d::Texture::MapToTexture(std::vector<vec4> data, int resolution)
{
	D3D11_MAPPED_SUBRESOURCE map{};
	map = Display::get()->m_device_context->GetMap(texture);
	vec4* destPtr = reinterpret_cast<vec4*>(map.pData);
	UINT pitch = resolution * sizeof(vec4);
	const vec4* sourcePtr = data.data(); // already vec4*
	for (UINT y = 0; y < resolution; y++) {
		std::memcpy(
			reinterpret_cast<uint8_t*>(destPtr) + y * map.RowPitch,
			sourcePtr + y * resolution,
			pitch
		);
	}
	Display::get()->m_device_context->UnMap(texture);
}

void dx3d::Texture::Load2DSmokeTexture(int resolution, bool t)
{
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Height = resolution;
	texDesc.Width = resolution;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (t == false) {
		texDesc.Usage = D3D11_USAGE_DYNAMIC;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (t == true) {
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		texDesc.CPUAccessFlags = 0;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex{};
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture2D(&texDesc, nullptr, tex.GetAddressOf()), "Create Smoke Texture Failed!");
	texture = tex;

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc{};
	shader_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_desc.Texture2D.MipLevels = 1;
	shader_desc.Texture2D.MostDetailedMip = 0;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(texture.Get(), &shader_desc, srv.GetAddressOf()), "Create shaderResourceView failed.");

	if (t)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
		uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uav_desc.Texture2D.MipSlice = 0;
		DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(texture.Get(), &uav_desc, uav.GetAddressOf()), "Create unorderedAccessView failed.");
	}
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, sampler.GetAddressOf()), "CreateSamplerState failed.");
}

void dx3d::Texture::Load3DSmokeTexture(int resolution)
{
	D3D11_TEXTURE3D_DESC texDesc{};
	texDesc.Height = resolution;
	texDesc.Width = resolution;
	texDesc.Depth = resolution;
	texDesc.MipLevels = 0;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	Microsoft::WRL::ComPtr<ID3D11Texture3D> tex{};
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture3D(&texDesc, nullptr, tex.GetAddressOf()), "Create Smoke Texture Failed!");
	texture = texture;

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc{};
	shader_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	shader_desc.Texture3D.MipLevels = -1;
	shader_desc.Texture3D.MostDetailedMip = 0;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(texture.Get(), &shader_desc, srv.GetAddressOf()), "Create shaderResourceView failed.");

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
	uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
	uav_desc.Texture3D.FirstWSlice = 0;
	uav_desc.Texture3D.MipSlice = 0;
	uav_desc.Texture3D.WSize = resolution;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(texture.Get(), &uav_desc, uav.GetAddressOf()), "Create unorderedAccessView failed.");
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MaxAnisotropy = 0;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, sampler.GetAddressOf()), "CreateSamplerState failed.");
}

void dx3d::Texture::Load3DFTex(int resolution)
{
	D3D11_TEXTURE3D_DESC texDesc{};
	texDesc.Height = resolution;
	texDesc.Width = resolution;
	texDesc.Depth = resolution;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	Microsoft::WRL::ComPtr<ID3D11Texture3D> tex{};
	DX3DGraphicsLogErrorAndThrow(m_device.CreateTexture3D(&texDesc, nullptr, tex.GetAddressOf()), "Create Smoke Texture Failed!");
	texture = tex;
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc{};
	shader_desc.Format = DXGI_FORMAT_R32_FLOAT;
	shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	shader_desc.Texture3D.MipLevels = 1;
	shader_desc.Texture3D.MostDetailedMip = 0;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateShaderResourceView(texture.Get(), &shader_desc, srv.GetAddressOf()), "Create shaderResourceView failed.");

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
	uav_desc.Format = DXGI_FORMAT_R32_FLOAT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
	uav_desc.Texture3D.FirstWSlice = 0;
	uav_desc.Texture3D.MipSlice = 0;
	uav_desc.Texture3D.WSize = resolution;
	DX3DGraphicsLogErrorAndThrow(m_device.CreateUnorderedAccessView(texture.Get(), &uav_desc, uav.GetAddressOf()), "Create unorderedAccessView failed.");
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	DX3DGraphicsLogErrorAndThrow(m_device.CreateSamplerState(&sampler_desc, sampler.GetAddressOf()), "CreateSamplerState failed.");
}
	
