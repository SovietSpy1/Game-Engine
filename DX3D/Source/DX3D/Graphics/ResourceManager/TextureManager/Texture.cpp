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
	m_device.CreateShaderResourceView(m_texture.Get(), &srv_desc, m_srv.GetAddressOf());
}
	
