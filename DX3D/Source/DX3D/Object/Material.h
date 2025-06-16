#pragma once
#include <DX3D/Graphics/GraphicsResource.h>

namespace dx3d {
	class Material : public GraphicsResource
	{
	public:
		Material(const GraphicsResourceDesc & desc);
		void SetPixelShader(const wchar_t* pixel_shader_path);
		void SetVertexShader(const wchar_t* vertex_shader_path);
		void SetTexture(const wchar_t* file_path);
		void AddTexture(const wchar_t* file_path);
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		RasterizerStateType rasterizerStateType{ RasterizerStateType::FrontFace };
		std::vector<std::shared_ptr<Texture>> textures{};
	};
}


