#include "DX3D/Graphics/ResourceManager/TextureManager/Texture.h"
#include <DirectXTex.h>
#include "DX3D/Graphics/GraphicsEngine.h"
#include <iostream>
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
	
