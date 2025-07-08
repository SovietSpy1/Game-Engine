#pragma once
#include <DX3D/Graphics/ResourceManager/Resource.h>
namespace dx3d {
	class Texture : public Resource
	{
	public:
		Texture(const wchar_t* full_path, const GraphicsResourceDesc& desc);
		Texture(const GraphicsResourceDesc& desc);
		void MapToTexture(std::vector<vec4> data, int resolution);
		void Load2DSmokeTexture(int resolution, bool t);
		void Load3DSmokeTexture(int resolution);
		void Load3DFTex(int resolution);
		Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler = nullptr;
	};
}

