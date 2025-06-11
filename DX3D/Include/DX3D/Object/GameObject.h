#pragma once
#include "DX3D/Core/Base.h"
#include <wrl.h>
namespace dx3d {
	class GameObject : public Base
	{
	public:
		GameObject(const BaseDesc& desc);
		void AddMesh(const wchar_t* file_path);
		void AddMesh();
		void SetTexture(const wchar_t* file_path);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetAxisBuffer();
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Texture> texture;
		Matrix4X4 transform;
		
		//Axis buffer for visualization
		bool showAxis = true;
		std::shared_ptr<VertexBuffer> axisVertexBuffer{};
	public:
		float zRot{ 0.0f };
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob{};
		Microsoft::WRL::ComPtr<ID3DBlob> pixelBlob{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader{};
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader{};
		friend class Display;
	};
}


