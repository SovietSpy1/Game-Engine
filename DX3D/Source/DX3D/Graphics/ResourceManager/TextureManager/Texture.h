#pragma once
#include <DX3D/Graphics/ResourceManager/Resource.h>
namespace dx3d {
	class Texture : public Resource
	{
	public:
		Texture(const wchar_t* full_path, const GraphicsResourceDesc& desc);
		Texture(int resolution, const GraphicsResourceDesc& desc);
	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> m_texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler = nullptr;
		friend class DeviceContext;
	};

}

